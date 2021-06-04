/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "ScanDataViewModel.h"

ScanDataViewModel::ScanDataViewModel(mechaspin::parakeet::ScanDataXY& scanDataXY) :
    scanData(scanDataXY)
{

}

const mechaspin::parakeet::ScanDataXY& ScanDataViewModel::getScanData()
{
    return scanData;
}
