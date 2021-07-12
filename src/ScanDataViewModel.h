/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#ifndef PARAKEET_QT_SCANDATAVIEWMODEL_H
#define PARAKEET_QT_SCANDATAVIEWMODEL_H

#include <parakeet/ScanDataPolar.h>
#include <parakeet/ScanDataXY.h>

class ScanDataViewModel
{
public:
    ScanDataViewModel(mechaspin::parakeet::ScanDataXY& scanDataXY);

    const mechaspin::parakeet::ScanDataXY& getScanData();

private:
    mechaspin::parakeet::ScanDataXY scanData;
};

#endif