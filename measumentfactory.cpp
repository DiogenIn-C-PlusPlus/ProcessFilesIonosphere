#include "measumentfactory.h"


std::unique_ptr<AlgorithmSkeleton> MeasumentRoute(RouteType type, RouteSettings settings)
{
    switch (type)
    {
        case RouteType::Cyprus:
        {
           return std::make_unique<Cyprus>(settings.time_window, settings.path_results + "/" + settings.name_trace_data, settings.path_experimental_file, settings.step_khz, settings.similary, settings.start_size_window_part_stan_dev, settings.type, settings.path_file_prognistic);
        }
        case RouteType::AlisSprings:
        {
           return std::make_unique<AlisSprings>(settings.time_window, settings.path_results + "/" + settings.name_trace_data, settings.path_experimental_file,  settings.step_khz, settings.similary, settings.start_size_window_part_stan_dev, settings.type, settings.path_file_prognistic);
        }
    default:
        return nullptr;
    }
}
