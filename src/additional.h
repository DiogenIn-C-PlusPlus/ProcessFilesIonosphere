#pragma once

#include <algorithm>
#include <filesystem>
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
    TimeFreq() = delete;
    explicit TimeFreq(double time_, double freq_)
        :time(time_),
         freq(freq_)
    {}
    double time;
    double freq;
};

struct PrognosticModes
{
    explicit PrognosticModes(std::map<size_t, size_t>&& number_hop_counts, size_t all_count)
        : number_hop_count(std::move(number_hop_counts)),
          all_count_modes(all_count)
    {}
    std::map<size_t, size_t> number_hop_count;
    size_t all_count_modes;
};

struct HopFreq
{
    size_t hop;
    double freq;
};

struct FileInfo
{
    explicit FileInfo(std::string&& name_file, std::string&& path_file)
        : name_file(std::move(name_file)), path_file(std::move(path_file))
    {}
    std::string name_file;
    std::string path_file;
};

constexpr size_t time_day_in_second = 24 * 3600;
constexpr size_t time_in_hour = 3600;
constexpr size_t coeff_translation_kHz_to_MHz = 1000;
constexpr double converse_time = 1000; // Для того, чтобы убрать хранение double в map для нивелирования ошибки вычислений (если ключ double)

void PrintVec(const std::vector<double>& vec, std::ostream& out);
void TranslateSymbolToZero(std::string& line,char symbol); // string_view?
std::vector<FileInfo> TakeAllFilesFromDir(const std::string& path_file);
