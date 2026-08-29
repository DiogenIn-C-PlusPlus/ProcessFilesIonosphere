#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

struct DataMof
{
    std::map<size_t, double> time_and_dispersion;
    std::map<size_t, double> time_and_middle_mof;
    std::map<size_t, double> time_and_standart_deviashion;
    std::map<size_t, std::vector<double>> time_per_sec_and_raw_mof;
    std::map<size_t, std::vector<double>> time_per_sec_and_filtered_mof;
    std::map<size_t, std::vector<double>> time_per_sec_and_selection_hist_mof;
};

struct DataPrognosticFreq
{
    std::map<size_t, std::vector<double>> time_freq;
    std::map<size_t, std::vector<double>> time_amplitude_in_files;
};

struct TimeFreq
{
    TimeFreq() = default;
    explicit TimeFreq(double time_, double freq_)
        :time(time_), freq(freq_)
    {}
    double time;
    double freq;
};


constexpr size_t time_day_in_second = 24 * 3600;
constexpr size_t time_in_hour = 3600;
constexpr size_t coeff_translation_kHz_to_MHz = 1000;

void PrintVec(const std::vector<double>& vec, std::ostream& out);
void TranslateSymbolToZero(std::string& line,char symbol); // string_view?
