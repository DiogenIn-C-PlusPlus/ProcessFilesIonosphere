#include "proccess.h"
#include <iomanip>
#include <cmath>

using namespace file;

Proccess::Proccess(std::unique_ptr<std::deque<FormatFileMof>> file_mof, uint16_t time_window)
    :file_mof_(std::move(file_mof)),
     time_window_(time_window)
{}

Proccess::Proccess(std::unique_ptr< std::vector<std::vector<FormatFilePrognosticRadioRouteAustralia>>> file_prognostic_route, size_t steps_mof, uint16_t detla_amplitude)
  :file_prognostic_route_australia_(std::move(file_prognostic_route)),
    steps_mof_(steps_mof),
    detla_amplitude_(detla_amplitude)
{}

Proccess::Proccess(std::unique_ptr<std::vector<std::vector<FormatFilePrognosticRadioRouteCyprus>>> file_prognostic_freq, size_t steps_mof, uint16_t delta_amplitude)
  : file_prognostic_route_cyprus_(std::move(file_prognostic_freq)),
    steps_mof_(steps_mof),
    detla_amplitude_(delta_amplitude)
{}

double Proccess::MiddleValue(const std::vector<double>& vec) const
{
    return vec.empty() ? 0 : std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
}

void Proccess::TakeInfoTimeAndMof()
{
    for(const auto& element : *file_mof_)
    {
        size_t time_key = (element.seconds / time_window_);
        mof_.time_per_sec_and_mof_[time_key].push_back(element.max_objective_freq);
    }
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

void PrintVec(const std::vector<double>& vec, std::ostream& out)   // Временная ф-я
{
    std::vector<double> temp_vec = vec;
    std::sort(temp_vec.begin(), temp_vec.end());
    for(size_t i = 0; i < temp_vec.size(); ++i)
    {
        out <<  temp_vec[i] << " ";
    }
    out  << std::endl;
}

void Proccess::ComputeTimeAndMiddleMof()
{
    for(const auto& [time_window, array_mor] : mof_.time_per_sec_and_mof_)
    {
        mof_.time_and_middle_mof_[time_window] = std::accumulate(array_mor.begin(), array_mor.end(), 0.0);
     //   mof_.time_and_middle_mof_[time_window] = MiddleValue(array_mor);
    }
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

void Proccess::ComputeTimeAndDispersionAndStandartDeviashionMof()
{
    // for(const auto& [time_window, array_mof] : mof_.time_per_sec_and_mof_)
    // {
    //        //auto middle_value = mof_.time_and_middle_mof_[time_window];
    //        auto middle_value = MiddleValue(array_mof);
    //        mof_.time_and_dispersion_[time_window] = ComputeDispersion(array_mof, middle_value);
    //        mof_.time_and_standart_deviashion_[time_window] = ComputeStandartDeviation(ComputeDispersion(array_mof, middle_value));
    // }
    for(const auto& [time_window, array_data] : save_time_per_sec_and_mof_)
    {
        std::vector<double> filtered_data;
        for(const double elem : array_data)
        {
            if(elem > 8000)
            {
                filtered_data.push_back(elem);
            }
        }
        double middle_value_probility = mof_.time_and_middle_mof_[time_window];
        mof_.time_and_dispersion_[time_window] = ComputeDispersion(filtered_data, middle_value_probility);
        mof_.time_and_standart_deviashion_[time_window] = ComputeStandartDeviation(ComputeDispersion(filtered_data, middle_value_probility));
    }
}

void Proccess::SelectionData(bool flag_cyprus_or_australia)
{
    std::ofstream out_before("show_values_mof_per_window_before.txt");
    std::ofstream out_after("show_values_mof_per_window_after.txt");
    for(auto& [time_window, array_mof] : mof_.time_per_sec_and_mof_)
    {
        save_time_per_sec_and_mof_[time_window] = array_mof;
        if(flag_cyprus_or_australia)
        {
            array_mof = SelectionDataCyprus(array_mof);
        }
        else
        {
            if(array_mof.size() < 7)
            {
                array_mof.clear();
                continue;
            }
            PrintVec(array_mof, out_before);
            array_mof = SelectionDataAustralia(array_mof);
            PrintVec(array_mof, out_after);
        }
    }
}

const std::unique_ptr<DataMof> Proccess::GetDataMof() const
{
    return std::make_unique<DataMof>(mof_);
}

void Proccess::SetTimeWindow(uint16_t time_per_second)
{
    time_window_ = time_per_second;
}

uint16_t Proccess::GetTimeWindow() const
{
    return time_window_;
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

const std::shared_ptr<DataPrognosticFreq> Proccess::GetDataPrognosticFreq() const
{
    return std::make_shared<DataPrognosticFreq>(prognostic_freq_);
}

std::vector<double> Proccess::SelectionDataCyprus(const std::vector<double> &vec) // Здесь идёт защита в зависимости от трассы, чтобы минимум и максимум
{                                                                                                                                   // не был меньше или через примерно большим, к примеру для Кипра меньше 8 Мгц (выкидываем априори)
    const double minimum_mof_cyprus = 8'000;
    const double maximum_mof_cyprus = 40'000;
    std::vector<double> filtered_mof;
    for(const double number : vec)
    {
        if(number > minimum_mof_cyprus && number < maximum_mof_cyprus)
        {
            filtered_mof.push_back(number);
        }
    }
        return BuildHistogramProbilityWindow(filtered_mof);
}

std::vector<double> Proccess::SelectionDataAustralia(const std::vector<double>& vec)
{
    const double minimum_mof_alis = 8'000;
    const double maximum_mof_alis = 50'000;
    std::vector<double> filtered_mof;
    for(const double number : vec)
    {
        if(number > minimum_mof_alis && number < maximum_mof_alis)
        {
            filtered_mof.push_back(number);
        }
    }
    //return BuildHistogramMaxWindow(filtered_mof);

    //PrintVec(BuildHistogramProbilityWindow(filtered_mof), std::cout);
    return BuildHistogramProbilityWindow(filtered_mof);
}

std::vector<double> Proccess::BuildHistogramEveryWindow(const std::vector<double>& filtered_mof)
{
    if(filtered_mof.empty())
    {
        return {};
    }
    std::vector<double> result;
    size_t similarity_current = 0; // Схожесть
    size_t similarity_need = filtered_mof.size() * 0.7;
    size_t coeff_increase_window_freq = 1;
    do
    {
        std::vector<Histogram> histogram;
        auto min_max = std::minmax_element(filtered_mof.begin(), filtered_mof.end());
        double stan_dev = (coeff_increase_window_freq / 10.2) * ComputeStandartDeviation(ComputeDispersion(filtered_mof, MiddleValue(filtered_mof)));
        double end_histog = *min_max.second;
        for(double start_histog = *min_max.first - stan_dev / 2; start_histog < end_histog; start_histog += stan_dev)
        {
            histogram.push_back(Histogram(start_histog));
        }
        for(const double freq : filtered_mof)
        {
            for(size_t i = 0; i < histogram.size(); ++i)
            {
                if(freq < histogram[i].freq_window + stan_dev / 2)
                {
                    histogram[i].collection_for_window.push_back(freq);
                    break;
                }
            }
        }
        auto max_elem = std::max_element(histogram.begin(), histogram.end(), [](const Histogram& lhs, const Histogram& rhs)
        {
            return lhs.collection_for_window.size() < rhs.collection_for_window.size();
        });
        if(max_elem == histogram.end())
        {
            return {};
        }
        similarity_current = max_elem->collection_for_window.size();
        ++coeff_increase_window_freq;
        result = max_elem->collection_for_window;
    } while (similarity_current < similarity_need);
    return result;
}

std::vector<double> Proccess::BuildHistogramMaxWindow(const std::vector<double>& filtered_mof)
{
    if(filtered_mof.empty())
    {
        return {};
    }
    std::vector<double> result;
    size_t similarity_current = 0; // Схожесть
    size_t similarity_need = filtered_mof.size() * 0.9;
    size_t coeff_increase_window_freq = 1;
    std::vector<Histogram> histogram;
    auto min_max = std::minmax_element(filtered_mof.begin(), filtered_mof.end());
    double stan_dev = (coeff_increase_window_freq / 5.2) * ComputeStandartDeviation(ComputeDispersion(filtered_mof, MiddleValue(filtered_mof)));
    std::cout << stan_dev << std::endl;
    double end_histog = *min_max.second;
    for(double start_histog = *min_max.first - stan_dev / 2; start_histog < end_histog; start_histog += stan_dev)
    {
       histogram.push_back(Histogram(start_histog));
    }
    for(const double freq : filtered_mof)
    {
       for(size_t i = 0; i < histogram.size(); ++i)
       {
          if(freq < histogram[i].freq_window + stan_dev / 2)
          {
             histogram[i].collection_for_window.push_back(freq);
             break;
          }
       }
     }
     auto max_elem = std::max_element(histogram.begin(), histogram.end(), [](const Histogram& lhs, const Histogram& rhs)
     {
        return lhs.collection_for_window.size() < rhs.collection_for_window.size();
     });
     if(max_elem == histogram.end())
     {
        return {};
     }
     double freq_with_max_count_window = max_elem->freq_window;
     do
     {
        double step_increase_window = stan_dev * coeff_increase_window_freq/ 10.1;
        double left_border = freq_with_max_count_window - step_increase_window;
        double right_border = freq_with_max_count_window + step_increase_window;
        std::vector<double> count_freqs_in_window;
        for(const double freq : filtered_mof)
        {
            if(freq > left_border && freq < right_border)
            {
                count_freqs_in_window.emplace_back(freq);
            }
        }
        similarity_current = count_freqs_in_window.size();
        ++coeff_increase_window_freq;
        result = count_freqs_in_window;
     } while (similarity_current < similarity_need);
    return result;
}

std::vector<double> Proccess::BuildHistogramProbilityWindow(const std::vector<double> &filtered_mof)
{
    if(filtered_mof.empty())
    {
        return {};
    }
    std::vector<double> result;
    size_t step = 1000; // 1 MHz start size window
    size_t count_elemets_time_window = filtered_mof.size();
    std::vector<Histogram> histogram;
    auto min_max = std::minmax_element(filtered_mof.begin(), filtered_mof.end());
    double end_histog = *min_max.second;
    for(double start_histog = *min_max.first - step / 2; start_histog < end_histog; start_histog += step)
    {
        histogram.push_back(Histogram(start_histog));
    }
    for(const double freq : filtered_mof)
    {
        for(size_t i = 0; i < histogram.size(); ++i)
        {
            if(freq < histogram[i].freq_window + step / 2)
            {
                histogram[i].collection_for_window.push_back(freq);
                break;
            }
        }
    }
    for(const auto& elem : histogram)
    {
        if(elem.collection_for_window.empty())
        {
            continue;
        }
        result.push_back(MiddleValue(elem.collection_for_window) * elem.collection_for_window.size() / count_elemets_time_window); // time window - это кол-во эл-тов всех во временном окне (все для данного набора частот)
    }
    return result;
}

void Proccess::TakeInfoTimeAndFreqPrognosticRouteAustralia() // Доделать исходя из посчитанного файла дома
{
    for(const auto& one_day : *file_prognostic_route_australia_)
    {
        std::vector<double> amplitude_per_step;
        size_t iterator = 0;
        for(const auto& element : one_day)
          {
            if(std::abs(element.amplitude_3 - 0) > 1e-3)
              {
                 amplitude_per_step.push_back(element.amplitude_3);
              }
            ++iterator;
              if(iterator == steps_mof_)
                {
                  std::sort(amplitude_per_step.begin(), amplitude_per_step.end());
                  if(!amplitude_per_step.empty())
                    {
                      std::vector<double> temp;
                      for(const double el : amplitude_per_step)
                        {
                          if(std::abs(amplitude_per_step.back() - el) < detla_amplitude_)
                            {
                              temp.push_back(el);
                            }
                        }
                      if(!temp.empty())
                        {
                           prognostic_freq_.time_amplitude_in_files[element.time_persent * coeff_for_double_time_to_int] = temp;
                        }
                    }
                  if(std::abs(element.freq_hop_5) - 0 < 1e-3)
                  {
                      prognostic_freq_.time_freq[element.time_persent * coeff_for_double_time_to_int].push_back(element.max_freq_hop);
                  }
                  else
                  {
                    prognostic_freq_.time_freq[element.time_persent * coeff_for_double_time_to_int].push_back(element.freq_hop_5);
                  }
                  amplitude_per_step.clear();
                  iterator = 0;
                }
          }
    }
}

void Proccess::TakeInfoTimeAndFreqPrognosticRouteCyprus()
{
  for(const auto& one_day : *file_prognostic_route_cyprus_)
  {
      std::vector<double> amplitude_per_step;
      size_t iterator = 0;
      for(const auto& element : one_day)
        {
          if(std::abs(element.amplitude_3 - 0) > 1e-3)
            {
               amplitude_per_step.push_back(element.amplitude_3);
            }
          ++iterator;
            if(iterator == steps_mof_)
              {
                std::sort(amplitude_per_step.begin(), amplitude_per_step.end());
                if(!amplitude_per_step.empty())
                  {
                    std::vector<double> temp;
                    for(const double el : amplitude_per_step)
                      {
                        if(std::abs(amplitude_per_step.back() - el) < detla_amplitude_)
                          {
                            temp.push_back(el);
                          }
                      }
                    if(!temp.empty())
                      {
                         prognostic_freq_.time_amplitude_in_files[element.time_persent * coeff_for_double_time_to_int] = temp;
                      }
                  }
                prognostic_freq_.time_freq[element.time_persent * coeff_for_double_time_to_int].push_back(element.max_freq_hop);
                amplitude_per_step.clear();
                iterator = 0;
              }
        }
  }
}

std::deque<TimeFreq> Proccess::ComputeMiddlePrognosticMof()
{
    std::deque<TimeFreq> result;
    for(const auto& [time, day_mof] : prognostic_freq_.time_freq)
      {
        result.push_back(TimeFreq{(static_cast<double>(time) / coeff_for_double_time_to_int), MiddleValue(day_mof)});
      }
    return result;
}
