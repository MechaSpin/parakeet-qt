/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "ProESetDestinationIPv4Dialog.h"
#include "ui_ProESetDestinationIPv4Dialog.h"

#include "../../IPv4Validator.h"

ProESetDestinationIPv4Dialog::ProESetDestinationIPv4Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProESetDestinationIPv4Dialog)
{
    ui->setupUi(this);
}

ProESetDestinationIPv4Dialog::~ProESetDestinationIPv4Dialog()
{
    delete ui;
}

QString ProESetDestinationIPv4Dialog::getIP()
{
    QString plainText = ui->ipAddressTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->ipAddressTextbox->placeholderText();
    }

    return plainText;
}

QString ProESetDestinationIPv4Dialog::getDstPort()
{
    QString plainText = ui->dstPortTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->dstPortTextbox->placeholderText();
    }

    return plainText;
}

void ProESetDestinationIPv4Dialog::on_ipAddressTextbox_textChanged()
{
    IPv4Validator::ensureValidAddressCharacters(ui->ipAddressTextbox);
}

void ProESetDestinationIPv4Dialog::on_dstPortTextbox_textChanged()
{
    IPv4Validator::ensureValidPortCharacters(ui->dstPortTextbox);
}

void ProESetDestinationIPv4Dialog::on_buttonBox_accepted()
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
