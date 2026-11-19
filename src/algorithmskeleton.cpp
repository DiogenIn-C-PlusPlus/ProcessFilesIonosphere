#include "algorithmskeleton.h"


AlgorithmSkeleton::AlgorithmSkeleton(uint16_t time_window_sec, const std::string& name_trace_data,  const std::string &path_experimental_file)
    : time_window_(time_window_sec),
      name_file_trace_and_data_(name_trace_data),
      path_experimental_file_(path_experimental_file)
{}

void AlgorithmSkeleton::MeasurmentRouteExperimentMof()
{
    ReadFileExperimentMof();
    BuildHistogramTimeMof();
    FilterDataMofAndTransformTimeToUt();
    SelectionDataHistogram();
    ComputeMiddleMof();
    ComputeDispersionAndStandartDeviashionMof();
    ShowResultsExpermentalMof();
    ShowAllFreqInWindowFiltered();
    ShowSmallestWindowData();
    ShowAllFreqsInPoints();
    ShowStandartDeviationInWindow();
}

void AlgorithmSkeleton::MeasurmentRouteModellingMofCyprus()
{
    ReadFileModellingMof();
    SelectionFreqAndAmplitude();
    PrintFilePrognosticFreq();
    PrintFilePrognosticAmplitude();
}

void AlgorithmSkeleton::MeasurmentRouteModellingMofAlis()
{
    ReadFileExperimentMof();   // experimental
    BuildHistogramTimeMof();
    FilterDataMofAndTransformTimeToUt();

    ReadFileModellingMof(); // prognostic
    SelectionPrognosticModes();
    MergePrognosticFreqAndExperimental();
    SelectionFreqAndAmplitude();
    ComputeMiddlePrognosticMof();
    PrintFilePrognosticFreq();
    PrintFilePrognosticAmplitude();
}

void AlgorithmSkeleton::ReadFileExperimentMof()
{
    std::ifstream in(path_experimental_file_, std::ios::in);
    if(!in.is_open())
    {
        std::cerr << "File not open -> adress readfile -> function Read()" << " current path = " << path_experimental_file_ << std::endl;
        throw std::invalid_argument("Not right pass file or file is not exist or not avaible maybe not have rule for reading");
    }
    while(!in.eof())
    {
        std::string line;
        std::getline(in, line);
        FormatFileMof temp;
        std::istringstream iss(line);
        iss >> temp.trace >> temp.data >> temp.time >> temp.hours >> temp.seconds >> temp.unknown_data >> temp.max_objective_freq >> temp.unknown_data_sec >> temp.time_delay;
        file_mof_.emplace_back(temp);
    }
    in.close();
}

void AlgorithmSkeleton::ShowResultsExpermentalMof()
{
    std::ofstream output(name_file_output_experimental_file_);
    double converse_time_to_hours = 1000;
    output << "\t   | Time |" << "\t\t| Middle mof |" << "\t| Middl - Stand def |" << "\t| Middl + Stand def |" << std::endl;
    for(const auto& [time, middle_mof] : mof_.time_and_middle_mof)
    {
        double stan_dev = mof_.time_and_standart_deviashion[time];
        output << "\t " <<  std::setw(8) << time / converse_time_to_hours  << "\t\t" << std::setw(10) << middle_mof / coeff_translation_kHz_to_MHz << "\t   " <<  std::setw(10) << (middle_mof -  stan_dev) / coeff_translation_kHz_to_MHz << "\t\t   " << std::setw(10) << (middle_mof + stan_dev) / coeff_translation_kHz_to_MHz << std::endl;
    }
}

void AlgorithmSkeleton::BuildHistogramTimeMof()
{
    for(const auto& element : file_mof_)
    {
        size_t time_key = (element.seconds / time_window_);
        mof_.time_per_sec_and_raw_mof[time_key].push_back(element.max_objective_freq);
    }
}

void AlgorithmSkeleton::ShowAllFreqInWindowFiltered()
{
    std::ofstream out(name_file_all_freq_filter_in_windows_);
    std::vector<double> temp_vec_for_sort;
    out << "\t[ Time ] \t\t\t\t" << "[ Freq in window ]" << std::endl;
    for(const auto& [time, collection_filtered_mof] : mof_.time_per_sec_and_filtered_mof)
    {
        temp_vec_for_sort = collection_filtered_mof;
        std::sort(temp_vec_for_sort.begin(), temp_vec_for_sort.end());
        out << std::setw(10) << "[ " << time / converse_time << " ] ";
        PrintVec(temp_vec_for_sort, out);
    }
}

void AlgorithmSkeleton::ShowSmallestWindowData()
{
    std::ofstream out(name_file_smallest_data_);
    std::vector<std::pair<double, size_t>> value_count_data_in_window;
    for(const auto& [time, collection_filtered_mof] : mof_.time_per_sec_and_filtered_mof)
    {
        value_count_data_in_window.push_back(std::make_pair(time, collection_filtered_mof.size()));
    }
    std::sort(value_count_data_in_window.begin(), value_count_data_in_window.end(), [] (const std::pair<double, size_t>& lhs, const std::pair<double, size_t>& rhs)
    {
        return lhs.second < rhs.second;
    });
    out << "\t[ Time in window ]\t\t[ Count values in window ]" << std::endl;
    for(size_t i = 0; i < count_low_data_in_window_ && i < value_count_data_in_window.size(); ++i)
    {
        out << std::setw(20) << "Time = " << value_count_data_in_window[i].first / converse_time << std::setw(20) << "\t count = " << value_count_data_in_window[i].second << std::endl;
    }
}

void AlgorithmSkeleton::ShowStandartDeviationInWindow()
{
    std::ofstream out(name_file_output_time_standart_deviation);
    std::set<std::pair<size_t, double>> sort_time_dev;
    out << "\t[ Time ]\t" << "[ Value standart deviation ]" << std::endl;
    for(const auto& [time, stand_dev] : mof_.time_and_standart_deviashion)
    {
        sort_time_dev.insert(std::make_pair(time / converse_time, stand_dev));
    }
    for(const auto& [time, stand_dev] : sort_time_dev)
    {
        out << std::setw(10) << "\t  [ " << time << " ]\t\t\t" << std::setw(10) << stand_dev / coeff_translation_kHz_to_MHz << std::endl;
    }
}

void AlgorithmSkeleton::PrintFilePrognosticFreq()
{
    std::ofstream output(name_file_prognostic_freq_);
    output << "\t| Time |" << "\t| Frequency |" << std::endl;
     for(const auto& element : time_freq_prognostic_result_)
     {
        output << "\t " <<  std::setw(5) << element.time  << "\t\t" << std::setw(10) << element.freq << '\n';
     }
}

void AlgorithmSkeleton::PrintFilePrognosticAmplitude()
{
    std::ofstream output(name_file_prognostic_amplitude_);
    output << "   | Time |" << "\t\t| Amplitude |"  << std::endl;
     for(const auto& [time, vec_freq_hour] : prognostic_data_.time_amplitude_in_files)
     {
         output  << "  " << std::setw(6) << time / converse_time << "\t" << std::setw(8) << vec_freq_hour.front() << "\t" << std::setw(8) << vec_freq_hour.back() << '\n';
     }
}

void AlgorithmSkeleton::ComputeMiddlePrognosticMof()
{
    for(const auto& [time, hour_mof] : prognostic_data_.time_freq)
    {
        time_freq_prognostic_result_.emplace_back(TimeFreq{(time / converse_time), (std::accumulate(hour_mof.begin(), hour_mof.end(), 0.0) / hour_mof.size())}); // Среднее всех частот в часе
    }
}
