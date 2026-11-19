#pragma once

#include <algorithm>
#include <vector>

#include "proccess.h"

enum class TypeHistogram
{
    Max,
    Probility,
    Every
};

struct Histogram
{
    explicit Histogram(double freq)
        : freq_window(freq), collection_for_window()
    {}
    double freq_window;
    std::vector<double> collection_for_window;
};

class BuildHistogram final
{
public:
    BuildHistogram() = delete;
    explicit BuildHistogram(size_t step_mhz, double similary, double start_size_window_part_stan_dev);
    template <typename T>
    std::vector<double> HistogramMaxWindow(const std::vector<T>& vec);
    template <typename T>
    std::vector<double> HistogramProbilityWindow(const std::vector<T>& vec);
    template <typename T>
    std::vector<double> HistogramEveryWindow(const std::vector<T>& vec);
    virtual ~BuildHistogram() = default;
private:
    size_t step_mhz_;
    double similary_;
    double start_size_window_part_stan_dev_;
};

template<typename T>
std::vector<double> BuildHistogram::HistogramEveryWindow(const std::vector<T>& vec)
{
    static_assert(std::is_same_v<T, double> || std::is_same_v<T, int32_t>, "T must be double or int32_t");
    if(vec.empty())
    {
        return {};
    }
    Proccess proc;
    std::vector<double> result;
    size_t similarity_current = 0; // Схожесть
    size_t similarity_need = vec.size() * similary_;
    double coeff_increase_window_freq = 1;
    do
    {
        std::vector<Histogram> histogram;
        auto min_max = std::minmax_element(vec.begin(), vec.end());
        double stan_dev = (coeff_increase_window_freq / start_size_window_part_stan_dev_) * proc.ComputeStandartDeviation(proc.ComputeDispersion(vec, proc.MiddleValue(vec)));
        double end_histog = *min_max.second;
        for(double start_histog = *min_max.first - stan_dev / 2; start_histog < end_histog; start_histog += stan_dev)
        {
            histogram.push_back(Histogram(start_histog));
        }
        for(const auto freq : vec)
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

template<typename T>
std::vector<double> BuildHistogram::HistogramMaxWindow(const std::vector<T>& vec)
{
    static_assert(std::is_same_v<T, double> || std::is_same_v<T, int32_t>, "T must be double or int32_t");
    if(vec.empty())
    {
        return {};
    }
    Proccess proc;
    std::vector<double> result;
    size_t similarity_current = 0; // Схожесть
    size_t similarity_need = vec.size() * similary_;
    double coeff_increase_window_freq = 1;
    std::vector<Histogram> histogram;
    auto min_max = std::minmax_element(vec.begin(), vec.end());
    double stan_dev = (coeff_increase_window_freq / start_size_window_part_stan_dev_) * proc.ComputeStandartDeviation(proc.ComputeDispersion(vec, proc.MiddleValue(vec)));
    double end_histog = *min_max.second;
    for(double start_histog = *min_max.first - stan_dev / 2; start_histog < end_histog; start_histog += stan_dev)
    {
       histogram.push_back(Histogram(start_histog));
    }
    for(const auto freq : vec)
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
        for(const auto freq : vec)
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

template<typename T>
std::vector<double> BuildHistogram::HistogramProbilityWindow(const std::vector<T>& vec)
{
    static_assert(std::is_same_v<T, double> || std::is_same_v<T, int32_t>, "T must be double or int32_t");
    if(vec.empty())
    {
        return {};
    }
    Proccess proc;
    std::vector<double> result;
    size_t count_elemets_time_window = vec.size();
    std::vector<Histogram> histogram;
    auto min_max = std::minmax_element(vec.begin(), vec.end());
    double end_histog = *min_max.second;
    for(double start_histog = *min_max.first - step_mhz_ / 2; start_histog < end_histog; start_histog += step_mhz_)
    {
        histogram.push_back(Histogram(start_histog));
    }
    for(const auto freq : vec)
    {
        for(size_t i = 0; i < histogram.size(); ++i)
        {
            if(freq < histogram[i].freq_window + step_mhz_)
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
        double probility_coeff = elem.collection_for_window.size() / static_cast<double>(count_elemets_time_window); // time window - это кол-во эл-тов всех во временном окне (все для данного набора частот)
        result.push_back(proc.MiddleValue(elem.collection_for_window) * probility_coeff);
    }
    return result;
}
