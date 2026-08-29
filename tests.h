#pragma once

#include <cassert>
#include <vector>
#include <string>

#include "proccess.h"
#include "readfile.h"
#include "outputfile.h"

class Tests
{
public:
    Tests();
private:
    void AllTests();
    void TestMiddleValue();
    void TestDispersion();
    void TestStandartDeviation();
    void TestChangeTimeToUt();
    void TestComputeStandartDeviation();
    void TestTakeInfoTimeAndMof();
    void TestComputeTimeAndMiddleMof();
    void TestComputeTimeAndDispersionMof();
    void TestSumFiles();
    void TestAddFile();
    void TestReadMof();
    void TestCalculateCountDataInWindowTime();
    void TestTransformTimeFromSeconds();
};










