#pragma once

#include "algorithmskeleton.h"
#include "buildhistogram.h"

class AlisSprings final : public AlgorithmSkeleton, public BuildHistogram
{
public:
    AlisSprings() = delete;
    explicit AlisSprings(uint16_t time_window, const std::string& name_trace_data, const std::string& path_experimental_file, size_t step_mhz, double similary, double start_size_window_part_stan_dev, TypeHistogram type, const std::string& path_file_prognistic);
    void FilterDataMofAndTransformTimeToUt() override;
    void SelectionDataHistogram() override;
    void ComputeMiddleMof() override;
    void ComputeDispersionAndStandartDeviashionMof() override;

    void ShowAllFreqsInPoints() override;

    void ReadFileModellingMof() override;
    void SelectionFreqAndAmplitude() override;
    void SelectionPrognosticModes() override; // test method (For correct prognostic MPF (max predicted frequency))
    void MergePrognosticFreqAndExperimental() override;
private:
    static constexpr size_t min_mof_ = 8'000;
    static constexpr size_t max_mof_ = 50'000;
    static constexpr size_t minimum_elements_ = 7;
    static constexpr size_t count_measurments_per_day_prognostic_ = 5;
    static constexpr size_t delta_amplitude_between_modes_ = 30;
    std::deque<FormatFilePrognosticRadioRouteAustralia> prognostic_route_;
    std::map<size_t, std::vector<HopFreq>> prognostic_time_and_number_hops_freq_;
    std::map<size_t, PrognosticModes> time_number_hop_count_modes_all_modes_;
    TypeHistogram type_;
    std::string path_file_prognostic_;
};
