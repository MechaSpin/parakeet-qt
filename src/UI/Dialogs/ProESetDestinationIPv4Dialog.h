/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#pragma once

#include <QDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class ProESetDestinationIPv4Dialog;
}

class ProESetDestinationIPv4Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProESetDestinationIPv4Dialog(QWidget *parent = nullptr);
    ~ProESetDestinationIPv4Dialog();

    QString getIP();
    QString getDstPort();

private slots:
    void on_ipAddressTextbox_textChanged();
    void on_dstPortTextbox_textChanged();
    void on_buttonBox_accepted();

private:
    Ui::ProESetDestinationIPv4Dialog *ui;

    static QString ipaddressAcceptedCharacters;
    static const int MAX_IPADDRESS_LENGTH = 15;
    static const int MAX_PORT_LENGTH = 5;

    QSettings settings;
};
