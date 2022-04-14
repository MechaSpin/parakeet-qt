/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#pragma once

#include <QDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class ProEConfigurationDialog;
}

class ProEConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProEConfigurationDialog(QWidget *parent = nullptr);
    ~ProEConfigurationDialog();

    QString getIP();
    QString getDstPort();
    QString getSrcPort();

private slots:
    void on_ipAddressTextbox_textChanged();

    void on_dstPortTextbox_textChanged();
    void on_srcPortTextbox_textChanged();

    void on_buttonBox_accepted();

private:
    Ui::ProEConfigurationDialog *ui;

    QSettings settings;
};
