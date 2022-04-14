/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

#include <parakeet/exceptions/NotConnectedToSensorException.h>
#include <parakeet/exceptions/UnableToDetermineBaudRateException.h>
#include <parakeet/exceptions/UnableToOpenPortException.h>

#include <parakeet/util.h>

#include "UI/Dialogs/ProEConfigurationDialog.h"
#include "UI/Dialogs/ProESetDestinationIPv4Dialog.h"
#include "UI/Dialogs/ProESetSourceIPv4Dialog.h"

#include "IPv4Validator.h"

#define AUTOMATIC_BAUD_RATE_TEXT "Auto"

#define MM_TO_M(mm) mm * 0.001
#define M_TO_MM(m) m * 1000

const int QGRAPHICSVIEW_WIDTH = 50000;
const int QGRAPHICSVIEW_HEIGHT = 50000;
const int QGRAPHICSVIEW_STARTING_ZOOM_MILLIMETERS = 5000;
const int QGRAPHICSVIEW_POLAR_CIRCLE_DIST_MILLIMETERS = 1000;

const std::string UNOFFICIAL_BAUD_RATE_MESSAGE_TITLE = "Unofficial Baud Rate Detected";
const std::string UNOFFICIAL_BAUD_RATE_MESSAGE_BODY_PRE = "An unofficial baud rate of ";
const std::string UNOFFICIAL_BAUD_RATE_MESSAGE_BODY_POST = " was detected on the sensor. Connection will proceed, but results may not be accurate.";

const QString PARAKEET_PRO_SENSOR_TITLE = "Parakeet Pro";
const QString PARAKEET_PROE_SENSOR_TITLE = "Parakeet ProE";
const QString FAILURE_TO_CONNECT_TO_SENSOR = "Unable to connect to sensor. Make sure the correct configuration is being used.";

const std::vector<mechaspin::parakeet::Driver::ScanningFrequency> PARAKEET_PRO_SUPPORTED_FREQUENCIES =
{
    mechaspin::parakeet::Driver::ScanningFrequency::Frequency_7Hz,
    mechaspin::parakeet::Driver::ScanningFrequency::Frequency_10Hz,
    mechaspin::parakeet::Driver::ScanningFrequency::Frequency_15Hz
};

const std::vector<mechaspin::parakeet::Driver::ScanningFrequency> PARAKEET_PROE_SUPPORTED_FREQUENCIES =
{
    mechaspin::parakeet::Driver::ScanningFrequency::Frequency_10Hz,
    mechaspin::parakeet::Driver::ScanningFrequency::Frequency_15Hz
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<std::shared_ptr<ScanDataViewModel>>();

    ui->setupUi(this);
    fpsLabel = new QLabel(QString("FPS: "), ui->statusBar);
    ui->statusBar->addWidget(fpsLabel);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->setBaudRateComboBox->addItem(QString::fromStdString(mechaspin::parakeet::BaudRates::br768000.toString()), mechaspin::parakeet::BaudRates::br768000.getValue());

    ui->sensorSelectionComboBox->addItem(PARAKEET_PROE_SENSOR_TITLE);
    ui->sensorSelectionComboBox->addItem(PARAKEET_PRO_SENSOR_TITLE);

    ui->sensorSelectionComboBox->setCurrentIndex(0);

    scene = new QGraphicsScene(-0.5 * QGRAPHICSVIEW_WIDTH, -0.5 * QGRAPHICSVIEW_HEIGHT, QGRAPHICSVIEW_WIDTH, QGRAPHICSVIEW_HEIGHT);
    scene->setBackgroundBrush(QBrush(Qt::gray));

    drawPolarGrid(QGRAPHICSVIEW_POLAR_CIRCLE_DIST_MILLIMETERS);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setZoomTo(220);
    graphicsViewZoomed();

    connectionState = false;

    connect(this, SIGNAL(sendScanData(const std::shared_ptr<ScanDataViewModel>&)), this, SLOT(onScanDataReceived(const std::shared_ptr<ScanDataViewModel>&)), Qt::QueuedConnection);
    connect(ui->graphicsView, &QCustomGraphicsView::onZoom, this, &MainWindow::graphicsViewZoomed, Qt::QueuedConnection);

    enableUIFromConnectionState(false);
}

MainWindow::~MainWindow()
{
    if(connectionState)
    {
        driver->stop();
        driver->close();
    }
    delete ui;
}

bool MainWindow::connectViaEthernet()
{
    ProEConfigurationDialog proEConfigurationDialog;

    if(proEConfigurationDialog.exec())
    {
        mechaspin::parakeet::ProE::Driver::SensorConfiguration sensorConfiguration;
        sensorConfiguration.ipAddress = proEConfigurationDialog.getIP().toStdString();
        sensorConfiguration.srcPort = proEConfigurationDialog.getSrcPort().toInt();
        sensorConfiguration.dstPort = proEConfigurationDialog.getDstPort().toInt();
        sensorConfiguration.intensity = ui->intensityCheckBox->isChecked();
        sensorConfiguration.scanningFrequency_Hz = mechaspin::parakeet::Driver::ScanningFrequency::Frequency_15Hz;
        sensorConfiguration.dataSmoothing = ui->dataSmoothingCheckbox->isChecked();
        sensorConfiguration.dragPointRemoval = ui->dragPointRemovalCheckBox->isChecked();
        sensorConfiguration.resampleFilter = true;

        mechaspin::parakeet::ProE::Driver* proEDriver = new mechaspin::parakeet::ProE::Driver();
        driver = proEDriver;

        proEDriver->connect(sensorConfiguration);

        enableUIForProE();

        return true;
    }

    return false;
}

bool MainWindow::connectViaSerial()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QStringList portNames;
    for(QSerialPortInfo &info : ports)
    {
        portNames << info.portName() + ": " + info.description();
    }

    bool ok;
    QString item = QInputDialog::getItem(this, "Select COM Port", "Available Ports: ", portNames, 0, false, &ok);
    if(!ok)
    {
        return false;
    }

#ifdef __linux__
    item = "/dev/" + item;
#endif

    QString portName = item.split(":").at(0);

    mechaspin::parakeet::Pro::Driver::SensorConfiguration sensorConfiguration;
    sensorConfiguration.comPort = portName.toStdString();
    sensorConfiguration.baudRate = mechaspin::parakeet::BaudRates::Auto;
    sensorConfiguration.intensity = ui->intensityCheckBox->isChecked();
    sensorConfiguration.scanningFrequency_Hz = static_cast<mechaspin::parakeet::Driver::ScanningFrequency>(ui->scanningFrequencyComboBox->currentData().toInt());
    sensorConfiguration.dataSmoothing = ui->dataSmoothingCheckbox->isChecked();
    sensorConfiguration.dragPointRemoval = ui->dragPointRemovalCheckBox->isChecked();

    mechaspin::parakeet::Pro::Driver* proDriver = new mechaspin::parakeet::Pro::Driver();
    driver = proDriver;

    proDriver->connect(sensorConfiguration);

    if(sensorConfiguration.baudRate == mechaspin::parakeet::BaudRates::Auto.getValue())
    {
        mechaspin::parakeet::BaudRate connectedAtBaudRate = proDriver->getBaudRate();

        bool unofficialBaudRate = connectedAtBaudRate.getValue() != mechaspin::parakeet::BaudRates::br768000.getValue();

        if(unofficialBaudRate && connectedAtBaudRate.getValue() != mechaspin::parakeet::BaudRates::Auto.getValue())
        {
            QString title = QString::fromStdString(UNOFFICIAL_BAUD_RATE_MESSAGE_TITLE);
            QString body = QString::fromStdString(UNOFFICIAL_BAUD_RATE_MESSAGE_BODY_PRE + connectedAtBaudRate.toString() + UNOFFICIAL_BAUD_RATE_MESSAGE_BODY_POST);
            QMessageBox::warning(this, title, body);
        }
    }

    enableUIForPro();

    return true;
}

void MainWindow::on_connectButton_clicked()
{
    if(connectionState)
    {
        driver->stop();
        driver->close();
        ui->connectButton->setText("Connect");
        connectionState = false;
        enableUIFromConnectionState(connectionState);
        return;
    }

    ui->connectButton->setText("Connecting...");
    QApplication::processEvents();

    QString selectedSensor = ui->sensorSelectionComboBox->currentText();

    try
    {
        if(selectedSensor == PARAKEET_PRO_SENSOR_TITLE)
        {
            connectionState = connectViaSerial();
        }
        else if(selectedSensor == PARAKEET_PROE_SENSOR_TITLE)
        {
            connectionState = connectViaEthernet();
        }

        enableUIFromConnectionState(connectionState);

        if(!connectionState)
        {
            ui->connectButton->setText("Connect");
            return;
        }

        driver->registerScanCallback(std::function<void(const mechaspin::parakeet::ScanDataPolar&)>
            (
                [&] (const mechaspin::parakeet::ScanDataPolar& scanData)
                {
                    std::vector<mechaspin::parakeet::PointXY> convertedList;

                    for(auto point : scanData.getPoints())
                    {
                        convertedList.push_back(mechaspin::parakeet::util::transform(point));
                    }

                    //It's important to emit data so that it gets synced back up on the GUI thread
                    mechaspin::parakeet::ScanDataXY scanDataXY(convertedList, scanData.getTimestamp());
                    emit sendScanData(std::shared_ptr<ScanDataViewModel>(new ScanDataViewModel(scanDataXY)));
                }
            )
            );

        driver->start();
        ui->connectButton->setText("Disconnect");
    }
    catch(const std::runtime_error&)
    {
        connectionState = false;
        enableUIFromConnectionState(connectionState);

        ui->connectButton->setText("Connect");

        QMessageBox box;
        box.setText(FAILURE_TO_CONNECT_TO_SENSOR);
        box.exec();

        driver->close();
    }
}

void MainWindow::enableUIFromConnectionState(bool state)
{
    ui->setSettingsButton->setEnabled(state);
}

void MainWindow::enableUIForPro()
{
    ui->resampleFilterCheckbox->setEnabled(false);
    ui->setBaudRateComboBox->setEnabled(true);
    ui->setBaudRateButton->setEnabled(true);
    ui->setSourceSettings->setEnabled(false);
    ui->setDestinationSettings->setEnabled(false);

    populateFrequencyList(PARAKEET_PRO_SUPPORTED_FREQUENCIES);
}

void MainWindow::enableUIForProE()
{
    ui->resampleFilterCheckbox->setEnabled(true);
    ui->setBaudRateComboBox->setEnabled(false);
    ui->setBaudRateButton->setEnabled(false);
    ui->setSourceSettings->setEnabled(true);
    ui->setDestinationSettings->setEnabled(true);

    populateFrequencyList(PARAKEET_PROE_SUPPORTED_FREQUENCIES);
}

void MainWindow::populateFrequencyList(const std::vector<mechaspin::parakeet::Driver::ScanningFrequency>& frequencies)
{
    ui->scanningFrequencyComboBox->clear();

    for(int i = 0; i < frequencies.size(); i++)
    {
        auto frequency = frequencies[i];
        ui->scanningFrequencyComboBox->addItem(QString::number(frequency), QVariant::fromValue(static_cast<int>(frequency)));

        if(frequency == mechaspin::parakeet::Driver::ScanningFrequency::Frequency_15Hz)
        {
            ui->scanningFrequencyComboBox->setCurrentIndex(i);
        }
    }
}

void MainWindow::onScanDataReceived(const std::shared_ptr<ScanDataViewModel>& data)
{
    for(auto point : pointsInQGraphicsView)
    {
        scene->removeItem(point);
    }

    pointsInQGraphicsView.clear();

    for(auto point : data->getScanData().getPoints())
    {
        pointsInQGraphicsView.push_back(scene->addEllipse(point.getX_mm() - dotSize, point.getY_mm() - dotSize, dotSize, dotSize, Qt::NoPen, QBrush(Qt::blue)));
    }

    fpsLabel->setText(QString("FPS: ") + QString::number(driver->getScanRate_Hz()));
}

void MainWindow::graphicsViewZoomed()
{
    dotSize = qMax(3 * (1 / ui->graphicsView->transform().m11()), (double)5);

    for(auto point : pointsInQGraphicsView)
    {
        auto rectangle = point->rect();
        rectangle.setWidth(dotSize);
        rectangle.setHeight(dotSize);
        point->setRect(rectangle);
    }
}

void MainWindow::drawPolarGrid(double metersPerCircle)
{
    QPen pen;  // creates a default pen
    pen.setWidth(0);
    pen.setBrush(Qt::darkGray);

    // Draw a circle every X meters
    for(int radius = metersPerCircle; radius <= QGRAPHICSVIEW_HEIGHT; radius += metersPerCircle)
    {
        scene->addEllipse(0 - (radius * 0.5), 0 - (radius * 0.5), radius, radius, pen);
    }

    // Draw Lines
    scene->addLine(0, -scene->height(), 0, scene->height());
    scene->addLine(-scene->width(), 0, scene->width(), 0);
}

void MainWindow::on_rangeSpinBox_valueChanged(int arg1)
{
    ui->graphicsView->resetToCenterPosition();
    ui->graphicsView->setZoomTo(M_TO_MM(arg1));
}

void MainWindow::on_setSettingsButton_clicked()
{
    mechaspin::parakeet::ProE::Driver* proEDriver = dynamic_cast<mechaspin::parakeet::ProE::Driver*>(driver);

    //Flush all the settings (excluding baud rate) to the sensor

    //Intensity Data
    driver->enableIntensityData(ui->intensityCheckBox->checkState());

    //Drag Point Removal
    driver->enableRemoveDragPoint(ui->dragPointRemovalCheckBox->checkState());

    //Data Smoothing
    driver->enableDataSmoothing(ui->dataSmoothingCheckbox->checkState());

    //Resample Filter
    if(proEDriver)
    {
        proEDriver->enableResampleFilter(ui->resampleFilterCheckbox->checkState());
    }

    //Scanning Frequency
    auto frequency = static_cast<mechaspin::parakeet::Driver::ScanningFrequency>(ui->scanningFrequencyComboBox->currentData().toInt());
    driver->setScanningFrequency_Hz(frequency);
}

void MainWindow::on_setBaudRateButton_clicked()
{
    //Baud Rate
    auto baudRate = mechaspin::parakeet::BaudRate(ui->setBaudRateComboBox->itemData(ui->setBaudRateComboBox->currentIndex()).toInt());

    mechaspin::parakeet::Pro::Driver* proDriver = dynamic_cast<mechaspin::parakeet::Pro::Driver*>(driver);
    if(proDriver)
    {
        proDriver->setBaudRate(baudRate);
    }
}

void MainWindow::on_setDestinationSettings_clicked()
{
    ProESetDestinationIPv4Dialog dialog;

    if(dialog.exec())
    {
        mechaspin::parakeet::ProE::Driver* proEDriver = dynamic_cast<mechaspin::parakeet::ProE::Driver*>(driver);
        if(proEDriver)
        {
            auto ipAddress = mechaspin::parakeet::util::addressToByteArray(dialog.getIP().toStdString());
            auto port = dialog.getDstPort().toUShort();
            proEDriver->setSensorDestinationIPv4Settings(ipAddress.data(),
                                                         port);
        }
    }
}

void MainWindow::on_setSourceSettings_clicked()
{
    ProESetSourceIPv4Dialog dialog;

    if(dialog.exec())
    {
        mechaspin::parakeet::ProE::Driver* proEDriver = dynamic_cast<mechaspin::parakeet::ProE::Driver*>(driver);
        if(proEDriver)
        {
            auto ipAddress = mechaspin::parakeet::util::addressToByteArray(dialog.getIP().toStdString());
            auto subnetMask = mechaspin::parakeet::util::addressToByteArray(dialog.getSubnetMask().toStdString());
            auto defaultGateway = mechaspin::parakeet::util::addressToByteArray(dialog.getDefaultGateway().toStdString());
            auto port = dialog.getPort().toUShort();

            proEDriver->setSensorIPv4Settings(ipAddress.data(),
                                                    subnetMask.data(),
                                                    defaultGateway.data(),
                                                    port);
        }
    }
}
