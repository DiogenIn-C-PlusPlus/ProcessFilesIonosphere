#pragma once

#include <algorithm>
#include <deque>
#include <numeric>
#include <map>
#include <memory>
#include <cmath>

#include "formatfiles.h"
#include "additional.h"
// maximum_objective_frequency == mof

class Proccess
{
public:
    Proccess() = default;

    uint32_t ChangeMoscowTimeToUtInSeconds(uint32_t time_moscow);
    double ChangeMoscowTimeToUtInHours(double time_moscow);
    double TransformTimeFromSeconds(double time_in_sec, double size_time_window);
    double MiddleValue(const std::vector<double>& vec) const;
    double ComputeDispersion(const std::vector<double>& values, double middle_value) const;
    double ComputeStandartDeviation(double dispersion) const;
};

