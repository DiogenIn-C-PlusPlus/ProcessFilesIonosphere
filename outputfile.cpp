#include "outputfile.h"

using namespace file;


OutPutFile::OutPutFile(std::shared_ptr<DataPrognosticFreq> data_prognostic)
    : data_prognostic_freq_(std::move(data_prognostic))
{}



void OutPutFile::PrintFilePrognosticFreq(std::string name_file, const std::deque<TimeFreq>& print_data)
{
   std::ofstream output(name_file);
   bool title = true;
    for(const auto& element : print_data)
    {
        if(title)
        {
            title = false;
            output << "\t| Time |" << "\t| Frequency |" << std::endl;
        }
       output << "\t " <<  std::setw(5) << element.time  << "\t\t" << std::setw(10) << element.freq << '\n';
    }
}

void OutPutFile::PrintFilePrognosticFreqAmplitude(std::string name_file) // Доработать на будущее возможность выводить несколько дней
{
  std::ofstream output(name_file);
  bool title = true;
   for(const auto& [time, vec_days] : data_prognostic_freq_->time_amplitude_in_files)
   {
       if(title)
       {
           title = false;
           output << "| Time |" << "\t\t\t| Amplitude |"  << std::endl;
       }
       output << static_cast<double>(time) / coeff_for_double_time_to_int;
       output << "\t" << std::setw(10) << vec_days.front() << "\t" << std::setw(10) << vec_days.back();
//       for(const double value : vec_days)
//         {
//              output << "\t" << std::setw(10) << value;
//         }
       output  << '\n';
   }
}

