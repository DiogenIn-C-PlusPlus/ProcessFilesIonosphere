#include "proccess.h"


double Proccess::MiddleValue(const std::vector<double>& vec) const
{
    return vec.empty() ? 0 : std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
}

uint32_t Proccess::ChangeMoscowTimeToUtInSeconds(uint32_t time_moscow)
{
    uint32_t delta = 3 * 3600;
     if(time_moscow <= delta)
    {
        return time_day_in_second  + time_moscow - delta;
    }
  return time_moscow - delta;
}

double Proccess::ComputeDispersion(const std::vector<double>& values, double middle_value) const
{
    if(values.empty() || values.size() == 1)
    {
        return 0;
    }
   return (std::accumulate(values.begin(), values.end(), 0.0, [middle_value](double& result, double temp)
                 {
                    return result += ((temp - middle_value) * (temp - middle_value));
                 })) / (values.size() - 1);
}

double Proccess::ComputeStandartDeviation(double dispersion) const
{
    return dispersion < 0.01 ? 0 : sqrt(dispersion);
}

double Proccess::ChangeMoscowTimeToUtInHours(double time_moscow)
{
    double delta = 3;
    double hours_in_day = 24;
    if(time_moscow < delta)
      {
        return hours_in_day + time_moscow - delta;
      }
    return time_moscow - delta;
}

double Proccess::TransformTimeFromSeconds(double time_in_sec, double size_time_window)
{
    return (time_in_sec *  size_time_window / time_in_hour) + size_time_window / (2 * time_in_hour); // Сумма прибавляется(чтобы был вывод не на краю границ временного окна, а в середине)
}
