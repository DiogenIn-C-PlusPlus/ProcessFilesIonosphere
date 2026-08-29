#include "measumentfactory.h"
#include "readfile.h"

#include "magic_enum.hpp"

void SolvePrognosticAlis() // Пронгоз, указывается только файл откуда брать данные
{
    RouteSettings settings_alis;
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Source";
    ReadFile read_file(path_to_proccess_files);
    read_file.TakeAllFilesFromDir();
    for(const auto& path_or_name_file : read_file.GetAllFilesInDir())
    {
        settings_alis.name_file_prognistic = path_or_name_file.path_file_;
        auto alis_prognostic = MeasumentRoute(RouteType::AlisSprings, settings_alis, path_or_name_file.name_file_);
        alis_prognostic->MeasurmentRouteModellingMof();
    }
    //settings_alis.name_file_prognistic = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/build/G-u041eu0442u043bu0430u0434u043au0430/Alis_dasf_04_25.txt";
}

void SolvePrognosticCyprus()
{
    RouteSettings settings_cyprus;
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Source";
    ReadFile read_file(path_to_proccess_files);
    read_file.TakeAllFilesFromDir();
    for(const auto& path_or_name_file : read_file.GetAllFilesInDir())
    {
        settings_cyprus.name_file_prognistic = path_or_name_file.path_file_;
        auto cyprus_prognostic = MeasumentRoute(RouteType::Cyprus, settings_cyprus, path_or_name_file.name_file_);
        cyprus_prognostic->MeasurmentRouteModellingMof();
    }
}

void SolveExperementalAlis()
{
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Source";
    RouteSettings settings;
    ReadFile read_file(path_to_proccess_files);
    read_file.TakeAllFilesFromDir();
    settings.time_window = 3600;
    settings.step_khz = 1000;
    settings.type = TypeHistogram::Probility;
    for(const auto& path_or_name_file : read_file.GetAllFilesInDir())
    {
        settings.name_file_output_experimental_file = path_or_name_file.name_file_ + "_" + static_cast<std::string>(magic_enum::enum_name(settings.type))  +  "_ " + std::to_string(settings.step_khz) + "kHz.txt";
        settings.name_experimental_file = path_or_name_file.path_file_;
        auto alis_experimental = MeasumentRoute(RouteType::AlisSprings, settings, path_or_name_file.name_file_);
        alis_experimental->MeasurmentRouteExperimentMof();
    }
}

void SolveExperementalCyprus()
{
    const std::string path_to_proccess_files = "/home/andrew_q/Projects/Diogen/Proc_Files_Ionosphere/Source";
    RouteSettings settings;
    ReadFile read_file(path_to_proccess_files);
    read_file.TakeAllFilesFromDir();
    settings.time_window = 1800;
    settings.step_khz = 1000;
    settings.type = TypeHistogram::Probility;
    for(const auto& path_or_name_file : read_file.GetAllFilesInDir())
    {
        settings.name_file_output_experimental_file = path_or_name_file.name_file_ + "_" + static_cast<std::string>(magic_enum::enum_name(settings.type))  +  "_ " + std::to_string(settings.step_khz) + "kHz.txt";
        settings.name_experimental_file = path_or_name_file.path_file_;
        auto cyprus_experimental = MeasumentRoute(RouteType::Cyprus, settings, path_or_name_file.name_file_);
        cyprus_experimental->MeasurmentRouteExperimentMof();
    }
}


int main()
{
    //SolvePrognosticAlis();
    SolveExperementalAlis();
    return 0;
}
