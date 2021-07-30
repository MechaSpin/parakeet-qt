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
    QString getLidarPort();
    QString getLocalPort();

private slots:
    void on_ipAddressTextbox_textChanged();

    void on_lidarPortTextbox_textChanged();
    void on_localPortTextbox_textChanged();

    void on_buttonBox_accepted();

private:
    bool isIPAddressValid();

    static bool isAllowedIPAddressCharacter(QChar c);
    static bool isAllowedPortCharacter(QChar c);

    void checkTextboxCharacterRequirements(QPlainTextEdit* textBox, short maxStringLength, std::function<bool(QChar)> characterRequirementFunction);

    Ui::ProEConfigurationDialog *ui;

    static QString ipaddressAcceptedCharacters;
    static const int MAX_IPADDRESS_LENGTH = 15;
    static const int MAX_PORT_LENGTH = 5;

    QSettings settings;
};
