/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "ProEConfigurationDialog.h"
#include "ui_ProEConfigurationDialog.h"

#include "../../IPv4Validator.h"

const QString SETTING_LAST_USED_IP_ADDRESS = "LastUsedIP";
const QString SETTING_LAST_USED_SRC_PORT = "LastUsedSrcPort";
const QString SETTING_LAST_USED_DST_PORT = "LastUsedDstPort";

ProEConfigurationDialog::ProEConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProEConfigurationDialog)
{
    ui->setupUi(this);

    if(settings.contains(SETTING_LAST_USED_IP_ADDRESS))
    {
        ui->ipAddressTextbox->insertPlainText(settings.value(SETTING_LAST_USED_IP_ADDRESS).toString());
    }
    if(settings.contains(SETTING_LAST_USED_DST_PORT))
    {
        ui->dstPortTextbox->insertPlainText(settings.value(SETTING_LAST_USED_DST_PORT).toString());
    }
    if(settings.contains(SETTING_LAST_USED_SRC_PORT))
    {
        ui->srcPortTextbox->insertPlainText(settings.value(SETTING_LAST_USED_SRC_PORT).toString());
    }
}

ProEConfigurationDialog::~ProEConfigurationDialog()
{
    settings.setValue(SETTING_LAST_USED_IP_ADDRESS, getIP());
    settings.setValue(SETTING_LAST_USED_DST_PORT, getDstPort());
    settings.setValue(SETTING_LAST_USED_SRC_PORT, getSrcPort());
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

QString ProEConfigurationDialog::getDstPort()
{
    QString plainText = ui->dstPortTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->dstPortTextbox->placeholderText();
    }

    return plainText;
}

QString ProEConfigurationDialog::getSrcPort()
{
    QString plainText = ui->srcPortTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->srcPortTextbox->placeholderText();
    }

    return plainText;
}

void ProEConfigurationDialog::on_ipAddressTextbox_textChanged()
{
    IPv4Validator::ensureValidAddressCharacters(ui->ipAddressTextbox);
}

void ProEConfigurationDialog::on_dstPortTextbox_textChanged()
{
    IPv4Validator::ensureValidPortCharacters(ui->dstPortTextbox);
}

void ProEConfigurationDialog::on_srcPortTextbox_textChanged()
{
    IPv4Validator::ensureValidPortCharacters(ui->srcPortTextbox);
}

void ProEConfigurationDialog::on_buttonBox_accepted()
{
    if(!IPv4Validator::isAddressValid(getIP()))
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
