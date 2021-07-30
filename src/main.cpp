/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QSettings qsettings("MechaSpin", "Parakeet Qt");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
