/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "ProEConfigurationDialog.h"
#include "ui_ProEConfigurationDialog.h"

const QString SETTING_LAST_USED_IP_ADDRESS = "LastUsedIP";
const QString SETTING_LAST_USED_LIDAR_PORT = "LastUsedLidarPort";
const QString SETTING_LAST_USED_LOCAL_PORT = "LastUsedLocalPort";

QString ProEConfigurationDialog::ipaddressAcceptedCharacters = ".locahst";

ProEConfigurationDialog::ProEConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProEConfigurationDialog)
{
    ui->setupUi(this);

    if(settings.contains(SETTING_LAST_USED_IP_ADDRESS))
    {
        ui->ipAddressTextbox->insertPlainText(settings.value(SETTING_LAST_USED_IP_ADDRESS).toString());
    }
    if(settings.contains(SETTING_LAST_USED_LIDAR_PORT))
    {
        ui->lidarPortTextbox->insertPlainText(settings.value(SETTING_LAST_USED_LIDAR_PORT).toString());
    }
    if(settings.contains(SETTING_LAST_USED_LOCAL_PORT))
    {
        ui->localPortTextbox->insertPlainText(settings.value(SETTING_LAST_USED_LOCAL_PORT).toString());
    }
}

ProEConfigurationDialog::~ProEConfigurationDialog()
{
    settings.setValue(SETTING_LAST_USED_IP_ADDRESS, getIP());
    settings.setValue(SETTING_LAST_USED_LIDAR_PORT, getLidarPort());
    settings.setValue(SETTING_LAST_USED_LOCAL_PORT, getLocalPort());
    delete ui;
}

QString ProEConfigurationDialog::getIP()
{
    QString plainText = ui->ipAddressTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->ipAddressTextbox->placeholderText();
    }

    return plainText;
}

QString ProEConfigurationDialog::getLidarPort()
{
    QString plainText = ui->lidarPortTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->lidarPortTextbox->placeholderText();
    }

    return plainText;
}

QString ProEConfigurationDialog::getLocalPort()
{
    QString plainText = ui->localPortTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->localPortTextbox->placeholderText();
    }

    return plainText;
}

void ProEConfigurationDialog::checkTextboxCharacterRequirements(QPlainTextEdit* textBox, short maxStringLength, std::function<bool(QChar)> characterRequirementFunction)
{
    bool requiresUpdate = false;
    QString newText = textBox->toPlainText();

    for(int i = newText.length() - 1; i >= 0; i--)
    {
        if(!characterRequirementFunction(newText[i]))
        {
            newText.remove(i, 1);
            requiresUpdate = true;
        }
    }

    if(newText.length() > maxStringLength)
    {
        newText = newText.remove(maxStringLength, newText.length() - maxStringLength);
        requiresUpdate = true;
    }

    if(requiresUpdate)
    {
        textBox->setPlainText(newText);
        textBox->moveCursor(QTextCursor::MoveOperation::End);
    }
}

bool ProEConfigurationDialog::isIPAddressValid()
{
    //IP Addresses follow A.B.C.D or localhost
    //where A/B/C/D are 1-3 digits in length

    QString ipaddress = getIP();

    if(ipaddress == "localhost")
    {
        return true;
    }
    else
    {
        QStringList periodDelimited = ipaddress.split('.');

        if(periodDelimited.size() != 4)
        {
            return false;
        }

        for(int i = 0; i < periodDelimited.size(); i++)
        {
            auto sectionLength = periodDelimited[i].length();
            if(sectionLength < 1 || sectionLength > 3)
            {
                return false;
            }

            for(int j = 0; j < sectionLength; j++)
            {
                if(!periodDelimited[i][j].isDigit())
                {
                    return false;
                }
            }
        }

        return true;
    }
}

bool ProEConfigurationDialog::isAllowedIPAddressCharacter(QChar c)
{
    return c.isDigit() || ipaddressAcceptedCharacters.contains(c);
}

bool ProEConfigurationDialog::isAllowedPortCharacter(QChar c)
{
    return c.isDigit();
}

void ProEConfigurationDialog::on_ipAddressTextbox_textChanged()
{
    checkTextboxCharacterRequirements(ui->ipAddressTextbox, MAX_IPADDRESS_LENGTH, &ProEConfigurationDialog::isAllowedIPAddressCharacter);
}

void ProEConfigurationDialog::on_lidarPortTextbox_textChanged()
{
    checkTextboxCharacterRequirements(ui->lidarPortTextbox, MAX_PORT_LENGTH, &ProEConfigurationDialog::isAllowedPortCharacter);
}

void ProEConfigurationDialog::on_localPortTextbox_textChanged()
{
    checkTextboxCharacterRequirements(ui->localPortTextbox, MAX_PORT_LENGTH, &ProEConfigurationDialog::isAllowedPortCharacter);
}

void ProEConfigurationDialog::on_buttonBox_accepted()
{
    if(!isIPAddressValid())
    {
        QMessageBox invalidIPMessageBox;
        invalidIPMessageBox.setText("Invalid IP Address\nUse form: A.B.C.D or localhost");
        invalidIPMessageBox.exec();
    }
    else
    {
        accept();
    }
}
