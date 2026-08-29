#include "measumentfactory.h"


std::unique_ptr<AlgorithmSkeleton> MeasumentRoute(RouteType type, RouteSettings settings, const std::string& name_trace_data)
{
    switch (type)
    {
        case RouteType::Cyprus:
        {
            return std::make_unique<Cyprus>(settings.time_window, name_trace_data, settings.name_experimental_file, settings.name_file_output_experimental_file, settings.step_khz, settings.similary, settings.start_size_window_part_stan_dev, settings.type, settings.name_file_prognistic);
        }
        case RouteType::AlisSprings:
        {
            return std::make_unique<AlisSprings>(settings.time_window, name_trace_data, settings.name_experimental_file, settings.name_file_output_experimental_file, settings.step_khz, settings.similary, settings.start_size_window_part_stan_dev, settings.type, settings.name_file_prognistic);
        }
    default:
        return nullptr;
    }
}
