#pragma once

#include <string_view>
#include <vector>
#include <deque>
#include <memory>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "formatfiles.h"

struct PathNameFile
{
    explicit PathNameFile(const std::string& name_file, const std::string& path_file)
        : name_file_(name_file), path_file_(path_file)
    {}
    std::string name_file_;
    std::string path_file_;
};

class ReadFile
{
public:
    ReadFile() = delete;
    explicit ReadFile(const std::string& path_file);
    void ChooseNameFiles(const std::string& path, const std::string& new_string);
    void TakeAllFilesFromDir();

    const std::vector<PathNameFile> &GetAllFilesInDir() const;
private:
    std::string path_file_;
    std::vector<PathNameFile> all_files_in_dir_;
};

