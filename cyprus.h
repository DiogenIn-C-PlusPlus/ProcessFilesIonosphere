#pragma once

#include "algorithmskeleton.h"
#include "buildhistogram.h"

class Cyprus  final : public AlgorithmSkeleton, public BuildHistogram
{
public:
    Cyprus() = delete;
    explicit Cyprus(uint16_t time_window, const std::string& name_trace_data, const std::string& name_experimental_file, const std::string& name_file_output_experimental_file, size_t step_mhz, double similary, double start_size_window_part_stan_dev, TypeHistogram type, const std::string& name_file_prognistic);
    void FilterDataMofAndTransformTimeToUt() override;
    void SelectionDataHistogram() override;
    void ComputeMiddleMof() override;
    void ComputeDispersionAndStandartDeviashionMof() override;

    void ReadFileModellingMof() override;
    void SelectionFreqAndAmplitude() override;
private:
    static constexpr size_t min_mof_ = 8'000;
    static constexpr size_t max_mof_ = 40'000;
    static constexpr size_t minimum_elements_ = 10;
    static constexpr size_t count_measurments_per_day_prognostic_ = 4;
    static constexpr size_t delta_amplitude_between_modes_ = 30;
    std::deque<FormatFilePrognosticRadioRouteCyprus> prognostic_route_;
    TypeHistogram type_;
    std::string name_file_prognostic_;
};
