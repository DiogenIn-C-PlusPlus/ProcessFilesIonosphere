#include <regex>

#include "measumentfactory.h"
#include "tests.h"

#include "magic_enum.hpp"

void SolvePrognosticAlis()     //settings_alis.name_file_prognistic = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/build/G-u041eu0442u043bu0430u0434u043au0430/Alis_dasf_04_25.txt";
{
    RouteSettings settings_alis;
    settings_alis.type = TypeHistogram::Probility; // для вероятностной гистограммы, не нужнен процент схожести и стартовый размер окна
    settings_alis.time_window = 3600;
    settings_alis.step_khz = 1000;
    const std::string path_to_proccess_prognostic_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Alis_modeling";
    const std::string path_to_proccess_exmerimental_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Alis_experiment";
    const std::string path_save_results = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Results_current/Alis_model";
    settings_alis.path_results = path_save_results;
    std::vector<FileInfo> files_prognostic = TakeAllFilesFromDir(path_to_proccess_prognostic_files);
    std::vector<FileInfo> files_experimental = TakeAllFilesFromDir(path_to_proccess_exmerimental_files);
    if(files_experimental.size() != files_prognostic.size())
    {
        throw std::invalid_argument("Check files in directory - prognostic and experimental");
    }
    for(size_t i = 0; i < files_prognostic.size(); ++i)
    {
        settings_alis.path_file_prognistic = files_prognostic[i].path_file;
        settings_alis.path_experimental_file = files_experimental[i].path_file;
        std::regex re(R"(_(\d{2})_(\d{2})\.txt$)");
        std::smatch match;
        if (std::regex_search(files_experimental[i].name_file, match, re))
        {
            settings_alis.name_trace_data = "Prognostic_" + static_cast<std::string>(magic_enum::enum_name(settings_alis.type))  + "_month_" + match[1].str()  + "_" + std::to_string(settings_alis.step_khz) + "kHz.txt";
        }
        auto alis_prognostic = MeasumentRoute(RouteType::AlisSprings, settings_alis);
        alis_prognostic->MeasurmentRouteModellingMofAlis();
    }
}

void SolveExperementalAlis(TypeHistogram type_hist, double similary = 0, double size_start_window_part_stan_dev = 0)
{
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Alis_experiment";
    const std::string path_save_results = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Results_current/Alis_exp";
    RouteSettings settings;
    settings.time_window = 3600;
    settings.step_khz = 1000;
    settings.type = type_hist;
    settings.path_results = path_save_results;
    if(type_hist == TypeHistogram::Every || type_hist == TypeHistogram::Max)
    {
        settings.similary = similary;
        settings.start_size_window_part_stan_dev = size_start_window_part_stan_dev;
    }
    for(const auto& elem : TakeAllFilesFromDir(path_to_proccess_files))
   {
        settings.name_trace_data = "Experimentral_" + elem.name_file + "_" + static_cast<std::string>(magic_enum::enum_name(settings.type))  +  "_" + std::to_string(settings.step_khz) + "kHz.txt";
        settings.path_experimental_file = elem.path_file;
        auto alis_experimental = MeasumentRoute(RouteType::AlisSprings, settings);
        alis_experimental->MeasurmentRouteExperimentMof();
    }
}

void SolvePrognosticCyprus()
{
    RouteSettings settings_cyprus;
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Kipr_modeling";
    const std::string path_save_results = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Results_current/Cyprus_model";
    settings_cyprus.step_khz = 1000;
    settings_cyprus.type = TypeHistogram::Probility;
    settings_cyprus.path_results = path_save_results;
    for(const auto& elem : TakeAllFilesFromDir(path_to_proccess_files))
    {
        settings_cyprus.path_file_prognistic = elem.path_file;
        std::regex re(R"(_(\d{2})_(\d{2})\.txt$)");
        std::smatch match;
        if (std::regex_search(elem.name_file, match, re))
        {
            settings_cyprus.name_trace_data = "Prognostic_" + static_cast<std::string>(magic_enum::enum_name(settings_cyprus.type))  + "_month_" + match[1].str()  + "_" + std::to_string(settings_cyprus.step_khz) + "kHz.txt";
        }
        auto cyprus_prognostic = MeasumentRoute(RouteType::Cyprus, settings_cyprus);
        cyprus_prognostic->MeasurmentRouteModellingMofCyprus();
    }
}

void SolveExperementalCyprus(TypeHistogram type_hist, double similary = 0, double size_start_window_part_stan_dev = 0)
{
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Kipr_experiment";
    const std::string path_save_results = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Results_current/Cyprus_exp";
    RouteSettings settings;
    settings.time_window = 1800;
    settings.step_khz = 3000;
    settings.type = type_hist;
    settings.path_results = path_save_results;
    if(type_hist == TypeHistogram::Every || type_hist == TypeHistogram::Max)
    {
        settings.similary = similary;
        settings.start_size_window_part_stan_dev = size_start_window_part_stan_dev;
    }
    for(const auto& elem : TakeAllFilesFromDir(path_to_proccess_files))
    {
        settings.name_trace_data = "Experimentral_" + elem.name_file + "_" + static_cast<std::string>(magic_enum::enum_name(settings.type))  +  "_" + std::to_string(settings.step_khz) + "kHz.txt";
        settings.path_experimental_file = elem.path_file;
        auto cyprus_experimental = MeasumentRoute(RouteType::Cyprus, settings);
        cyprus_experimental->MeasurmentRouteExperimentMof();
    }
}



int main()
{
 //   auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/proc_iono.log", 1024 * 1024 * 3, 3);

    //SolveExperementalCyprus(TypeHistogram::Probility);
//   SolveExperementalCyprus(TypeHistogram::Every, 0.4, 10);
//    SolvePrognosticCyprus();
   // SolveExperementalCyprus(TypeHistogram::Max, 0.4, 10);
    Tests tests;
    tests.AllTests();
    // SolvePrognosticAlis();

    // SolveExperementalAlis(TypeHistogram::Probility);

    return 0;
}
