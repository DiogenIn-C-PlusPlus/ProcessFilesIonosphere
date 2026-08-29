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
    std::string name_experimental_file;
    std::string name_file_output_experimental_file;
    std::string name_file_prognistic;
    double similary;
    double start_size_window_part_stan_dev;
    size_t step_khz;
    TypeHistogram type;
    uint16_t time_window;
};

std::unique_ptr<AlgorithmSkeleton> MeasumentRoute(RouteType type, RouteSettings settings, const std::string& name_trace_data);
