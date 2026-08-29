#include "readfile.h"



ReadFile::ReadFile(const std::string& path_file)
    : path_file_(path_file)
{}

      //      std::filesystem::rename(file.path(), file.path().parent_path() / (new_string + file.path().filename().string())); // первый аргумент это путь файла - текущее, второй указываем сначала директорию куда (родительскую), дальше идет через / название нового файла                                                                                                                                                             // если родительскую не указывать будет сохранять в папку программы
void ReadFile::ChooseNameFiles(const std::string& path, const std::string& new_string)
{
    for(const auto& file : std::filesystem::directory_iterator(path))
      { // 19 between 23
        if((file.path().filename().string().at(4) == '1' && file.path().filename().string().at(5) == '9') || (file.path().filename().string().at(4) == '2' && file.path().filename().string().at(5) == '0'))
          {
            std::filesystem::rename(file.path(), (new_string + file.path().filename().string())); // первый аргумент это путь файла - текущее, второй указываем сначала директорию куда (родительскую), дальше идет через / название нового файла                                                                                                                                                             // если родительскую не указывать будет сохранять в папку программы
          }
        else if((file.path().filename().string().at(4) == '2' && file.path().filename().string().at(5) == '2') || (file.path().filename().string().at(4) == '2' && file.path().filename().string().at(5) == '1'))
          {
            std::filesystem::rename(file.path(), (new_string + file.path().filename().string()));
          }
        else if((file.path().filename().string().at(4) == '2' && file.path().filename().string().at(5) == '3'))
          {
            std::filesystem::rename(file.path(), (new_string + file.path().filename().string()));
          }
    }
}

void ReadFile::TakeAllFilesFromDir()
{
    for(const auto& name_file : std::filesystem::directory_iterator(path_file_))
    {
        if(!std::filesystem::is_directory(name_file))
        {
            all_files_in_dir_.emplace_back(PathNameFile{name_file.path().filename(), name_file.path()});
        }
    }
}

const std::vector<PathNameFile> &ReadFile::GetAllFilesInDir() const
{
    return all_files_in_dir_;
}

