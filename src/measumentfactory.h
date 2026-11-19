#pragma once

#include <memory>

#include "cyprus.h"
#include "alissprings.h"

enum class RouteType
{
    Cyprus,
    AlisSprings
};

struct RouteSettings
{
    std::string path_experimental_file;
    std::string name_trace_data;
    std::string path_file_prognistic;
    std::string path_results;
    double similary;
    double start_size_window_part_stan_dev;
    size_t step_khz;
    TypeHistogram type;
    uint16_t time_window;
};

std::unique_ptr<AlgorithmSkeleton> MeasumentRoute(RouteType type, RouteSettings settings);
