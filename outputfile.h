#pragma once

#include <algorithm>
#include <sstream>
#include <cstdint>
#include <iostream>

#include "proccess.h"

namespace file
{
struct PrintFileMofExperiment
{
    double time;
    double middle_mof;
    double middle_mof_mines_stand_dev;
    double middle_mof_plus_stand_dev;
};

class OutPutFile
{
public:
//     OutPutFile() = default;
//     explicit OutPutFile(std::shared_ptr<DataPrognosticFreq> data_prognostic);

//     void PrintFileMof(std::string name_file, double size_time_window);
//     void PrintFilePrognosticFreq(std::string name_file, const std::deque<TimeFreq>& print_data);
//     void PrintFilePrognosticFreqAmplitude(std::string name_file);

// private:
//     void CalculateCountDataInWindowTime();
//     std::shared_ptr<DataPrognosticFreq> data_prognostic_freq_;
//     std::vector<uint16_t> count_points_in_time_window_;
};

}

