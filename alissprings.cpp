#include "alissprings.h"


AlisSprings::AlisSprings(uint16_t time_window, const std::string& name_trace_data, const std::string &path_experimental_file, size_t step_mhz, double similary, double start_size_window_part_stan_dev, TypeHistogram type, const std::string &path_file_prognistic)
    : AlgorithmSkeleton(time_window, name_trace_data, path_experimental_file),
      BuildHistogram(step_mhz, similary, start_size_window_part_stan_dev),
      type_(type),
      path_file_prognostic_(path_file_prognistic)
{}

void AlisSprings::FilterDataMofAndTransformTimeToUt()
{
    std::vector<double> filtered_mof;
    for(const auto& [time, mof] : mof_.time_per_sec_and_raw_mof)
    {
        auto key = Proccess::ChangeMoscowTimeToUtInHours(Proccess::TransformTimeFromSeconds(time, time_window_)) * converse_time;
        filtered_mof.clear();
        filtered_mof.reserve(mof.size());
        std::copy_if(mof.begin(), mof.end(), std::back_insert_iterator(filtered_mof), [this](double elem)
                     {
                         return elem > min_mof_ && elem < max_mof_;
                     });
        if(filtered_mof.size() > minimum_elements_)
        {
            mof_.time_per_sec_and_filtered_mof.insert_or_assign(key, std::move(filtered_mof));
        }
        else
        {
            mof_.time_per_sec_and_filtered_mof.insert_or_assign(key, std::vector<double>{});
        }
    }
}

void AlisSprings::SelectionDataHistogram()
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

void AlisSprings::ComputeMiddleMof()
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

void AlisSprings::ComputeDispersionAndStandartDeviashionMof()
{
    for(const auto& [time, filtered_collection] : mof_.time_per_sec_and_filtered_mof)
    {
        double middle_value = mof_.time_and_middle_mof[time];
        double dispersion_value = Proccess::ComputeDispersion(filtered_collection, middle_value);
        mof_.time_and_dispersion[time] = dispersion_value;
        mof_.time_and_standart_deviashion[time] = Proccess::ComputeStandartDeviation(dispersion_value);
    }
}

void AlisSprings::ReadFileModellingMof()
{
    const char symbol = 'n'; // По умолчанию в dasf файле выводится n, мы её меняем на 0
    std::ifstream in(path_file_prognostic_, std::ios::in);
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
        TranslateSymbolToZero(line, symbol);
        FormatFilePrognosticRadioRouteAustralia temp;
        std::istringstream iss(line);
        iss >> temp.time >> temp.time_persent >> temp.freq_persent_mof >> temp.amplitude_1 >> temp.amplitude_2 >> temp.amplitude_3 >> temp.amplitude_4 >> temp.amplitude_5 >> temp.amplitude_6;
        iss >> temp.distance >> temp.unknow_time >> temp.count_hops;
        iss >>  temp.freq_hop_1 >> temp.freq_hop_2 >> temp.freq_hop_3 >> temp.freq_hop_4;
        iss >> temp.freq_hop_5 >> temp.freq_hop_6 >> temp.freq_hop_7 >> temp.freq_hop_8 >> temp.freq_hop_9;
        iss >> temp.freq_hop_10 >> temp.freq_hop_11 >> temp.freq_hop_12 >> temp.freq_hop_13 >> temp.freq_hop_14 >> temp.freq_hop_15 >> temp.freq_hop_16 >> temp.freq_hop_17 >> temp.freq_hop_18;
        iss >> temp.freq_hop_19 >> temp.freq_hop_20 >> temp.max_freq_hop >> temp.freq_hop_22 >> temp.freq_hop_23 >> temp.unknow_data;
        prognostic_route_.emplace_back(temp);
    }
}

void AlisSprings::SelectionFreqAndAmplitude()
{
    std::vector<double> amplitude_per_hour;
    size_t current_measurment_per_hour = 0;
    const size_t count_scanning_hops = 20; // selection freq
    for(const auto& freq_ampl_hour_part : prognostic_route_)
    {
        const double* first_hop = &freq_ampl_hour_part.freq_hop_1;
        for(size_t i = 0; i < count_scanning_hops; ++i, ++first_hop)
        {
            if(*first_hop - 0 > 1e-4)
            {
                size_t time = freq_ampl_hour_part.time_persent * converse_time;
                prognostic_data_.time_freq[time].push_back(*first_hop * time_number_hop_count_modes_all_modes_.at(time).number_hop_count[i + 1]/ time_number_hop_count_modes_all_modes_.at(time).all_count_modes); // Почему +1?
            }
        }
        if(std::abs(freq_ampl_hour_part.amplitude_3 - 0) > 1e-4) // selection amplitude
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

static std::ostream& operator<<(std::ostream& os,
                                const FormatFilePrognosticRadioRouteAustralia& d)
{
    os << d.time              << ' '
       << d.time_persent      << ' '
       << d.freq_persent_mof  << ' '
       << d.amplitude_1       << ' '
       << d.amplitude_2       << ' '
       << d.amplitude_3       << ' '
       << d.amplitude_4       << ' '
       << d.amplitude_5       << ' '
       << d.amplitude_6       << ' '
       << d.distance          << ' '
       << d.unknow_time       << ' '
       << d.count_hops        << ' '
       << d.freq_hop_1        << ' '
       << d.freq_hop_2        << ' '
       << d.freq_hop_3        << ' '
       << d.freq_hop_4        << ' '
       << d.freq_hop_5        << ' '
       << d.freq_hop_6        << ' '
       << d.freq_hop_7        << ' '
       << d.freq_hop_8        << ' '
       << d.freq_hop_9        << ' '
       << d.freq_hop_10       << ' '
       << d.freq_hop_11       << ' '
       << d.freq_hop_12       << ' '
       << d.freq_hop_13       << ' '
       << d.freq_hop_14       << ' '
       << d.freq_hop_15       << ' '
       << d.freq_hop_16       << ' '
       << d.freq_hop_17       << ' '
       << d.freq_hop_18       << ' '
       << d.freq_hop_19       << ' '
       << d.freq_hop_20       << ' '
       << d.max_freq_hop      << ' '
       << d.freq_hop_22       << ' '
       << d.freq_hop_23       << ' '
       << d.unknow_data;
    return os;
}

// Основная функция записи в файл
bool WritePrognosticRadioRouteAustralia(
        const std::string& filename,
        const std::deque<FormatFilePrognosticRadioRouteAustralia>& prognostic_route_,
        bool append = false,
        char delimiter = '\n')
{
    std::ios::openmode mode = std::ios::out;
    if (append)
        mode |= std::ios::app;

    std::ofstream fout(filename, mode);
    if (!fout.is_open())
        return false;

    // Заголовок (можно убрать, если не нужен)
    fout << "time time_persent freq_persent_mof "
            "amplitude_1 amplitude_2 amplitude_3 amplitude_4 amplitude_5 amplitude_6 "
            "distance unknow_time count_hops "
            "freq_hop_1 freq_hop_2 freq_hop_3 freq_hop_4 freq_hop_5 "
            "freq_hop_6 freq_hop_7 freq_hop_8 freq_hop_9 freq_hop_10 "
            "freq_hop_11 freq_hop_12 freq_hop_13 freq_hop_14 freq_hop_15 "
            "freq_hop_16 freq_hop_17 freq_hop_18 freq_hop_19 freq_hop_20 "
            "max_freq_hop freq_hop_22 freq_hop_23 unknow_data\n";

    for (const auto& rec : prognostic_route_)
    {
        fout << rec << delimiter;
    }

    fout.close();
    return true;
}

void AlisSprings::SelectionPrognosticModes()
{
    constexpr size_t count_scanning_hops = 20;
    constexpr double delta_mhz_between_modes = 5;
    bool flag_clear = false;
    for(size_t counter_for_scip_extra_data = 0; counter_for_scip_extra_data < prognostic_route_.size(); ++counter_for_scip_extra_data)
    {
        if(counter_for_scip_extra_data % count_measurments_per_day_prognostic_ != 0)
        {
            continue;
        }
        const double* first_hop = &prognostic_route_[counter_for_scip_extra_data].freq_hop_1;
        for(size_t i = 0; i < count_scanning_hops; ++i, ++first_hop)
        {
            if(*first_hop - 0 > 1e-4)
            {
                if(prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].size() > 1 && std::abs((*first_hop - prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time][prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].size() - 2].freq)) < 1e-3) // Случай для одинаковых частот подряд
                {
                    continue;
                }
                if(flag_clear && *first_hop < prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time][prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].size() - 2].freq) // here mistake случай, когда идут частоты, потом n
                {
                    flag_clear = false;
                    prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].clear();
                }
                if(prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].size() > 1 && std::abs((prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time][prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].size() - 2].freq) - *first_hop) > delta_mhz_between_modes) // Случай, когда частоты вначале битые (гораздо меньше последующих)
                {
                    continue;
                }
                prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].emplace_back(HopFreq{(i + 1),*first_hop});
            }
            else
            {
                if(!prognostic_time_and_number_hops_freq_[prognostic_route_[counter_for_scip_extra_data].time_persent * converse_time].empty())
                {
                    flag_clear = true;
                }
            }
        }
    }
}

void AlisSprings::MergePrognosticFreqAndExperimental()
{
    std::vector<double> delta_window_between;
    std::map<size_t, size_t> number_hop_count_modes;
    size_t all_modes = 0;
    size_t current_number_hop = 0;
    for(const auto& [time, number_hop_freq] : prognostic_time_and_number_hops_freq_)
    {
        if(mof_.time_per_sec_and_filtered_mof[time].empty())
        {
            continue;
        }
        for(size_t i = 0; i < number_hop_freq.size() - 1 && number_hop_freq.size() > 1; ++i)
        {
            delta_window_between.push_back(std::abs(number_hop_freq[i].freq - number_hop_freq[i + 1].freq));
        }
        std::sort(mof_.time_per_sec_and_filtered_mof[time].begin(), mof_.time_per_sec_and_filtered_mof[time].end(), [](double lhs, double rhs)
        {
            return rhs > lhs;
        });
        double point_start_window = mof_.time_per_sec_and_filtered_mof[time][current_number_hop]; // изначальная позиция для данного временного окна
        for(size_t i = 0, number_window = 0; i < mof_.time_per_sec_and_filtered_mof[time].size(); ++i)
        {
            double current_value_freq = mof_.time_per_sec_and_filtered_mof[time][i];
            double shift_current_freq_about_delta_window = point_start_window - current_value_freq;
            double prognostic_delta_between_window = 0;
            if(number_window >= delta_window_between.size())
            {
                ++number_hop_count_modes[number_hop_freq[++current_number_hop].hop];
                continue;
            }
            else
            {
                prognostic_delta_between_window = delta_window_between[number_window];
            }
            if(shift_current_freq_about_delta_window < prognostic_delta_between_window)
            {
                ++number_hop_count_modes[number_hop_freq[current_number_hop++].hop]; // текущее и смещаем на следующее
            }
            else
            {
                if(i == number_hop_freq.size() - 1) // Последнее просто добавляем и выходим
                {
                    ++number_hop_count_modes[number_hop_freq[++current_number_hop].hop];
                    ++all_modes;
                    break;
                }
                if(shift_current_freq_about_delta_window < delta_window_between[number_window] + delta_window_between[number_window + 1]) // вопрос границ
                {
                    point_start_window -= delta_window_between[number_window];
                    ++number_window;
                    ++number_hop_count_modes[number_hop_freq[++current_number_hop].hop]; // берем сразу следующее так как она из следующего окна
                }
                else
                {
                    size_t shift_number_window = 0;
                    double sum_shift_delta_window = 0;
                    for(size_t i = number_window; i < delta_window_between.size(); ++i, ++current_number_hop)
                    {
                        sum_shift_delta_window += delta_window_between[i];
                        if(sum_shift_delta_window > shift_current_freq_about_delta_window)
                        {
                            sum_shift_delta_window -= delta_window_between[i];
                            point_start_window -= sum_shift_delta_window;
                            ++number_hop_count_modes[number_hop_freq[--current_number_hop].hop];
                            number_window = i - 1; // здесь проблемы нет по поводу границ изначально уже будет на 1 больше исходя из предыдущего условия
                            break;
                        }
                    }
                }
            }
        ++all_modes;
        }
        current_number_hop = 0;
        all_modes = 0;
        time_number_hop_count_modes_all_modes_.insert_or_assign(time, PrognosticModes{std::move(number_hop_count_modes), all_modes});
        number_hop_count_modes.clear();
    }
}

void AlisSprings::ShowAllFreqsInPoints()
{
    std::vector<double> temp_vec_for_sort;
    std::ofstream out(name_file_all_freq_in_points_);
    out << "\t[ Time ] \t\t\t\t" << "[ Points freq ]" << std::endl;
    for(const auto& [time, collection_filtered_mof] : mof_.time_per_sec_and_raw_mof)
    {
        temp_vec_for_sort = collection_filtered_mof;
        std::sort(temp_vec_for_sort.begin(), temp_vec_for_sort.end());
        out << std::setw(10) << "[ " << Proccess::ChangeMoscowTimeToUtInHours(Proccess::TransformTimeFromSeconds(time, time_window_)) * converse_time / converse_time << " ] ";
        for(const double elem : temp_vec_for_sort)
        {
            out << " " << std::setw(9) << elem;
        }
        out << std::endl;
    }
}
