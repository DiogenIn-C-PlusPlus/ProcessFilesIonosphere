#include "cyprus.h"


Cyprus::Cyprus(uint16_t time_window, const std::string& name_trace_data, const std::string &name_experimental_file, const std::string &name_file_output_experimental_file, size_t step_mhz, double similary, double start_size_window_part_stan_dev, TypeHistogram type, const std::string &name_file_prognistic)
    : AlgorithmSkeleton(time_window, name_trace_data, name_experimental_file, name_file_output_experimental_file),
      BuildHistogram(step_mhz, similary, start_size_window_part_stan_dev),
      type_(type),
      name_file_prognostic_(name_file_prognistic)
{}

void Cyprus::FilterDataMofAndTransformTimeToUt()
{
    std::vector<double> filtered_mof;
    double converse_time_for_hours = 1000;
    for(const auto& [time, mof] : mof_.time_per_sec_and_raw_mof)
    {
        auto key = Proccess::ChangeMoscowTimeToUtInHours(Proccess::TransformTimeFromSeconds(time, time_window_)) * converse_time_for_hours;
        if(mof.size() > minimum_elements_)
        {
            filtered_mof.clear();
            filtered_mof.reserve(mof.size());
            std::copy_if(mof.begin(), mof.end(), std::back_insert_iterator(filtered_mof), [this](double elem)
                         {
                             return elem > min_mof_ && elem < max_mof_;
                         });
            mof_.time_per_sec_and_filtered_mof.insert_or_assign(key, std::move(filtered_mof));
        }
        else
        {
            mof_.time_per_sec_and_filtered_mof.insert_or_assign(key, std::vector<double>{});
        }
    }
}

void Cyprus::SelectionDataHistogram()
{
    switch (type_)
    {
        case TypeHistogram::Max:
        {
            for(const auto& [time, collection_mof] : mof_.time_per_sec_and_filtered_mof)
            {
                mof_.time_per_sec_and_selection_hist_mof[time] = BuildHistogram::HistogramMaxWindow(collection_mof);
            }
            break;
        }
        case TypeHistogram::Probility:
        {
            for(const auto& [time, collection_mof] : mof_.time_per_sec_and_filtered_mof)
            {
                if(time == 11500)
                {
                    std::string name;
                }
                mof_.time_per_sec_and_selection_hist_mof[time] = BuildHistogram::HistogramProbilityWindow(collection_mof);
            }
            break;
        }
        case TypeHistogram::Every:
        {
            for(const auto& [time, collection_mof] : mof_.time_per_sec_and_filtered_mof)
            {
                mof_.time_per_sec_and_selection_hist_mof[time] = BuildHistogram::HistogramEveryWindow(collection_mof);
            }
            break;
        }
    default:
        {
            std::cerr << "Unknown type histogram" << std::endl;
        }
    }
}

void Cyprus::ComputeMiddleMof()
{
    switch (type_)
    {
        case TypeHistogram::Probility:
        {
            for(const auto& [time, collection_after_hist_mof] : mof_.time_per_sec_and_selection_hist_mof)
            {
                mof_.time_and_middle_mof[time] = std::accumulate(collection_after_hist_mof.begin(), collection_after_hist_mof.end(), 0.0);
            }
            break;
        }
        case TypeHistogram::Every:
            [[fallthrough]];
        case TypeHistogram::Max:
        {
            for(const auto& [time, collection_after_hist_mof] : mof_.time_per_sec_and_selection_hist_mof)
            {
                mof_.time_and_middle_mof[time] = Proccess::MiddleValue(collection_after_hist_mof);
            }
            break;
        }
        default:
        {
            std::cerr << "Unknown type histogram" << std::endl;
        }
    }
}

void Cyprus::ComputeDispersionAndStandartDeviashionMof()
{
    for(const auto& [time, filtered_collection] : mof_.time_per_sec_and_filtered_mof)
    {
        double middle_value = mof_.time_and_middle_mof[time];
        double dispersion_value = Proccess::ComputeDispersion(filtered_collection, middle_value);
        mof_.time_and_dispersion[time] = dispersion_value;
        mof_.time_and_standart_deviashion[time] = Proccess::ComputeStandartDeviation(dispersion_value);
    }
}

void Cyprus::ReadFileModellingMof()
{
    std::ifstream in(name_file_prognostic_, std::ios::in);
    if(!in.is_open())
    {
        std::cerr << "File not open -> adress readfile -> function Read()" << std::endl;
        throw std::invalid_argument("Not right pass file or file is not exist or not avaible maybe not have rule for reading");
    }
    while(!in.eof())
    {
        std::string line;
        std::getline(in, line);
        if(line.empty())
        {
          continue;
        }
        FormatFilePrognosticRadioRouteCyprus temp;
        std::istringstream iss(line);
        iss >> temp.time >> temp.time_persent >> temp.freq_persent_mof >> temp.amplitude_1 >> temp.amplitude_2 >> temp.amplitude_3 >> temp.amplitude_4 >> temp.amplitude_5 >> temp.amplitude_6;
        iss >> temp.freq >> temp.unknow_time >> temp.count_hops;
        iss >>  temp.freq_hop_1 >> temp.freq_hop_2 >> temp.freq_hop_3 >> temp.freq_hop_4 >> temp.max_freq_hop >> temp.freq_hop_5 >> temp.freq_hop_6 >> temp.unknow_data;
        prognostic_route_.emplace_back(temp);
    }
}

void Cyprus::SelectionFreqAndAmplitude()
{
    std::vector<double> amplitude_per_hour;
    size_t current_measurment_per_hour = 0;
    double converse_time = 1000; // Для того, чтобы убрать хранение double в map для нивелирования ошибки вычислений (если ключ double)
    for(const auto& freq_ampl_hour_part : prognostic_route_)
    {
        prognostic_data_.time_freq[freq_ampl_hour_part.time_persent * converse_time].push_back(freq_ampl_hour_part.freq_hop_4);
        if(std::abs(freq_ampl_hour_part.amplitude_3 - 0) > 1e-4)
        {
            amplitude_per_hour.push_back(freq_ampl_hour_part.amplitude_3);
        }
        if(current_measurment_per_hour == count_measurments_per_day_prognostic_)
        {
            current_measurment_per_hour = 0;
            if(!amplitude_per_hour.empty())
            {
                std::sort(amplitude_per_hour.begin(), amplitude_per_hour.end());
                double max_elem = amplitude_per_hour.back();
                std::vector<double> selection_amplitude;
                for(const double elem : amplitude_per_hour)
                {
                    if(std::abs(max_elem - elem) < delta_amplitude_between_modes_)
                    {
                        selection_amplitude.push_back(elem);
                    }
                }
                if(!selection_amplitude.empty())
                {
                    prognostic_data_.time_amplitude_in_files[freq_ampl_hour_part.time_persent * converse_time] = selection_amplitude;
                }
                amplitude_per_hour.clear();
            }
        }
        else
        {
            ++current_measurment_per_hour;
        }
    }
}
