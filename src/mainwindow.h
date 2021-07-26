/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#ifndef PARAKEET_QT_MAINWINDOW_H
#define PARAKEET_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QFont>
#include <QtGlobal>

#include <parakeet/ProE/Driver.h>
#include <parakeet/Pro/Driver.h>
#include <parakeet/util.h>

#include "ScanDataViewModel.h"

Q_DECLARE_METATYPE(std::shared_ptr<ScanDataViewModel>);

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButtonSerial_clicked();
    void on_connectButtonEthernet_clicked();
    void onScanDataReceived(const std::shared_ptr<ScanDataViewModel>& data);
    void graphicsViewZoomed();
    void on_rangeSpinBox_valueChanged(int arg1);
    void on_setSettingsButton_clicked();
    void on_setBaudRateButton_clicked();
signals:
    void sendScanData(const std::shared_ptr<ScanDataViewModel>& data);

private:
    void drawPolarGrid(double metersPerCircle);
    void enableUIFromConnectionState(bool state);

    QGraphicsScene *scene;
    QLabel* fpsLabel;
    Ui::MainWindow *ui;
    mechaspin::parakeet::Driver* driver;

    bool connectionState;

    int zoomLevel_mm;
    double dotSize;

    std::vector<QGraphicsEllipseItem *> pointsInQGraphicsView;
};
#endif // MAINWINDOW_H
