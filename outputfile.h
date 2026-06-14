#pragma once

#include "proccess.h"
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <iostream>

namespace file
{

struct PrintFileMofExperiment
{
    double time;
    double middle_mof;
    double middle_mof_mines_stand_dev;
    double middle_mof_plus_stand_dev;
};

class OutPutFile : public Proccess
{
public:
    OutPutFile() = default; // Тестовая версия (боевая delete)
    explicit OutPutFile(std::unique_ptr<DataMof> data);
    explicit OutPutFile(std::shared_ptr<DataPrognosticFreq> data_prognostic);

    void PrintFileMof(std::string name_file, double size_time_window);
    void PrintFilePrognosticFreq(std::string name_file, const std::deque<TimeFreq>& print_data);
    void PrintFilePrognosticFreqAmplitude(std::string name_file);
    void ShowMinimumsColomsDataMof(uint16_t count_minimums);
    void PrintCountDataInWindowTime();
private: // тестировочный режим in comment
    std::string AddSpace(double size_time_window);
    void CalculateCountDataInWindowTime();
    double TransformTimeFromSeconds(double time_in_sec, double size_time_window);
    std::unique_ptr<DataMof> data_mof_;
    std::shared_ptr<DataPrognosticFreq> data_prognostic_freq_;
    std::vector<uint16_t> count_points_in_time_window_;
};

}

