#pragma once

#include <cstdint>
#include <deque>
#include <fstream>
#include <string>
#include <string_view>
#include <sstream>
#include <set>
#include <iomanip>
#include <numeric>

#include "additional.h"
#include "formatfiles.h"


class AlgorithmSkeleton
{
public:
    AlgorithmSkeleton() = delete;
    explicit AlgorithmSkeleton(uint16_t time_window_sec, const std::string& name_trace_data, const std::string& name_experimental_file);
    void MeasurmentRouteExperimentMof();
    void MeasurmentRouteModellingMofCyprus();
    void MeasurmentRouteModellingMofAlis();
    virtual ~AlgorithmSkeleton() = default;
protected:
    virtual void FilterDataMofAndTransformTimeToUt() = 0; // Experimental
    virtual void SelectionDataHistogram() = 0;
    virtual void ComputeMiddleMof() = 0;
    virtual void ComputeDispersionAndStandartDeviashionMof() = 0;

    virtual void ReadFileModellingMof() = 0; // Prognostic
    virtual void SelectionFreqAndAmplitude() = 0;

    virtual void SelectionPrognosticModes() = 0;
    virtual void MergePrognosticFreqAndExperimental() = 0;

    virtual void ShowAllFreqsInPoints() = 0;  // General

    void ReadFileExperimentMof(); // Experimental
    void ShowResultsExpermentalMof();
    void BuildHistogramTimeMof();
    void ShowAllFreqInWindowFiltered();
    void ShowSmallestWindowData();
    void ShowStandartDeviationInWindow();

    void PrintFilePrognosticFreq(); // Prognostic
    void PrintFilePrognosticAmplitude();
    void ComputeMiddlePrognosticMof();

    std::deque<FormatFileMof> file_mof_;
    std::deque<TimeFreq> time_freq_prognostic_result_;
    DataMof mof_;
    DataPrognosticFreq prognostic_data_;
    uint16_t time_window_;

private:
    static constexpr size_t count_low_data_in_window_ = 10;
    const std::string name_file_trace_and_data_;
    const std::string path_experimental_file_;

protected:
    const std::string name_file_smallest_data_ = name_file_trace_and_data_ + "_Smallest_data_window.txt";
    const std::string name_file_prognostic_freq_ = name_file_trace_and_data_ + "_Prognostic_freq.txt";
    const std::string name_file_prognostic_amplitude_ = name_file_trace_and_data_ + "_Prognostic_amplitude.txt";
    const std::string name_file_all_freq_filter_in_windows_ = name_file_trace_and_data_ + "_Filtered_all_modes.txt";
    const std::string name_file_all_freq_in_points_ = name_file_trace_and_data_ + "_All_modes.txt";
    const std::string name_file_output_experimental_file_ = name_file_trace_and_data_ + "_Middle_Mof_stand_dev.txt";
    const std::string name_file_output_time_standart_deviation = name_file_trace_and_data_ + "_Time_Stand_Dev.txt";
    static constexpr size_t count_measurments_per_day_prognostic_ = 5;
};
