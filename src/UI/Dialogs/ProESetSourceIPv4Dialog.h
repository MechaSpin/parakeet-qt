/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#pragma once

#include <QDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class ProESetSourceIPv4Dialog;
}

class ProESetSourceIPv4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProESetSourceIPv4Dialog(QWidget *parent = nullptr);
    ~ProESetSourceIPv4Dialog();

    QString getIP();
    QString getSubnetMask();
    QString getDefaultGateway();
    QString getPort();

private slots:
    void on_ipAddressTextbox_textChanged();
    void on_subnetMask_textChanged();
    void on_defaultGateway_textChanged();
    void on_port_textChanged();

    void on_buttonBox_accepted();

private:
    Ui::ProESetSourceIPv4Dialog *ui;

    static QString ipaddressAcceptedCharacters;
    static const int MAX_IPADDRESS_LENGTH = 15;
    static const int MAX_PORT_LENGTH = 5;

    QSettings settings;
};
