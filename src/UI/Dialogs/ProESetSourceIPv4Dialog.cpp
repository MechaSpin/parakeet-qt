/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "ProESetSourceIPv4Dialog.h"
#include "ui_ProESetSourceIPv4Dialog.h"

#include "../../IPv4Validator.h"

ProESetSourceIPv4Dialog::ProESetSourceIPv4Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProESetSourceIPv4Dialog)
{
    ui->setupUi(this);
}

ProESetSourceIPv4Dialog::~ProESetSourceIPv4Dialog()
{
    delete ui;
}

QString ProESetSourceIPv4Dialog::getIP()
{
    QString plainText = ui->ipAddressTextbox->toPlainText();

    if(plainText == "")
    {
        return ui->ipAddressTextbox->placeholderText();
    }

    return plainText;
}

QString ProESetSourceIPv4Dialog::getPort()
{
    QString plainText = ui->port->toPlainText();

    if(plainText == "")
    {
        return ui->port->placeholderText();
    }

    return plainText;
}

QString ProESetSourceIPv4Dialog::getDefaultGateway()
{
    QString plainText = ui->defaultGateway->toPlainText();

    if(plainText == "")
    {
        return ui->defaultGateway->placeholderText();
    }

    return plainText;
}

QString ProESetSourceIPv4Dialog::getSubnetMask()
{
    QString plainText = ui->subnetMask->toPlainText();

    if(plainText == "")
    {
        return ui->subnetMask->placeholderText();
    }

    return plainText;
}

void ProESetSourceIPv4Dialog::on_ipAddressTextbox_textChanged()
{
    IPv4Validator::ensureValidAddressCharacters(ui->ipAddressTextbox);
}

void ProESetSourceIPv4Dialog::on_subnetMask_textChanged()
{
    IPv4Validator::ensureValidAddressCharacters(ui->subnetMask);
}

void ProESetSourceIPv4Dialog::on_defaultGateway_textChanged()
{
    IPv4Validator::ensureValidAddressCharacters(ui->defaultGateway);
}

void ProESetSourceIPv4Dialog::on_port_textChanged()
{
    IPv4Validator::ensureValidPortCharacters(ui->port);
}

void ProESetSourceIPv4Dialog::on_buttonBox_accepted()
{
    if(!IPv4Validator::isAddressValid(getIP()))
    {
        QMessageBox invalidIPMessageBox;
        invalidIPMessageBox.setText("Invalid IP Address\nUse form: A.B.C.D or localhost");
        invalidIPMessageBox.exec();
    }
    else if(!IPv4Validator::isAddressValid(getDefaultGateway()))
    {
        QMessageBox invalidDefaultGateway;
        invalidDefaultGateway.setText("Invalid Default Gateway\nUse form: A.B.C.D");
        invalidDefaultGateway.exec();
    }
    else if(!IPv4Validator::isAddressValid(getSubnetMask()))
    {
        QMessageBox invalidSubnetMask;
        invalidSubnetMask.setText("Invalid Subnet Mask\nUse form: A.B.C.D");
        invalidSubnetMask.exec();
    }
    else
    {
        accept();
    }
}
