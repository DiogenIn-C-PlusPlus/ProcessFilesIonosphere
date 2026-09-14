#include "additional.h"

void PrintVec(const std::vector<double>& vec, std::ostream& out)
{
    std::vector<double> temp_vec = vec;
    std::sort(temp_vec.begin(), temp_vec.end());
    for(size_t i = 0; i < temp_vec.size(); ++i)
    {
        out <<  temp_vec[i] << " ";
    }
    out  << std::endl;
}

void TranslateSymbolToZero(std::string& line, char symbol)
{
  for(char& c : line)
    {
      if(c == symbol)
        {
          c = 48;
        }
    }
}

std::vector<FileInfo> TakeAllFilesFromDir(const std::string& path_file)
{
    std::vector<FileInfo> name_file_and_path_in_dir;
    for(const auto& name_file : std::filesystem::directory_iterator(path_file))
    {
        if(!std::filesystem::is_directory(name_file))
        {
            name_file_and_path_in_dir.emplace_back(name_file.path().filename().string(), name_file.path().string());
        }
    }
    std::sort(name_file_and_path_in_dir.begin(), name_file_and_path_in_dir.end(), [] (const FileInfo& lhs, const FileInfo& rhs)
    {
        return lhs.name_file < rhs.name_file;
    });
    return name_file_and_path_in_dir;
}
