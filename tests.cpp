#include "tests.h"

Tests::Tests()
{
    AllTests();
}

void Tests::AllTests()
{
//    TestMiddleValue();
//    TestDispersion();
//    TestStandartDeviation();
//    TestChangeTimeToUt();
//    TestComputeStandartDeviation();
//    TestTakeInfoTimeAndMof();
//    TestComputeTimeAndMiddleMof();
//    TestComputeTimeAndDispersionMof();
//    TestSumFiles();
//    TestAddFile();
//    TestReadMof();
//    TestCalculateCountDataInWindowTime();
//    TestTransformTimeFromSeconds();
}
/*
void Tests::TestMiddleValue()
{
    using namespace file;
    std::vector<double> numbers = {1,2,3,4,5,6,7,8,9,10}; // Подсчет
    Proccess prosses;
    assert(std::abs(5.5 - prosses.MiddleValue(numbers)) < 1e-5);
    std::vector<double> numbers1; // empty
    assert(std::abs(0 - prosses.MiddleValue(numbers1)) < 1e-5);
    std::vector<double> numbers2{2};
    assert(std::abs(2 - prosses.MiddleValue(numbers2)) < 1e-5);
}

void Tests::TestDispersion()
{
    using namespace file;
    Proccess proccess;
    std::vector<double> numbers = {12,23,45,33,65,54,54};
    double middle = proccess.MiddleValue(numbers);
    assert(std::abs(40.857 - middle)  < 1e-3);
    double dispersion = proccess.ComputeDispersion(numbers, middle);
    assert(std::abs(359.81 - dispersion) < 1e-2); // Заменить на СКО
    std::vector<double> test;
    assert(std::abs(0 - proccess.ComputeDispersion(test, 0)) < 1e-5);
}

void Tests::TestStandartDeviation()
{
    using namespace file;
    Proccess proccess;
    std::vector<double> numbers = {12,23,45,33,65,54,54};
    double middle = proccess.MiddleValue(numbers);
    assert(std::abs(40.857 - middle)  < 1e-3);
    double dispersion = proccess.ComputeDispersion(numbers, middle);
    assert(std::abs(sqrt(359.81) - proccess.ComputeStandartDeviation(dispersion)) < 1e-2); // Заменить на СКО
    assert(std::abs(0 - proccess.ComputeStandartDeviation(-1)) < 1e-5);
}

void Tests::TestChangeTimeToUt()
{
    using namespace file;
    Proccess proccess;
    uint32_t time_early = 7000;
    assert(proccess.ChangeMoscowTimeToUt(time_early) == 17800);
    uint32_t time_late = 82800;
    assert(proccess.ChangeMoscowTimeToUt(time_late) == 7200);
}

void Tests::TestComputeStandartDeviation()
{
    using namespace file;
    Proccess proccess;
    std::vector<double> numbers = {12,23,45,33,65,54,54};
    double middle = proccess.MiddleValue(numbers);
    assert(std::abs(40.857 - middle)  < 1e-3);
    double dispersion = proccess.ComputeDispersion(numbers, middle);
    assert(std::abs(359.81 - dispersion) < 1e-2);
    std::vector<double> numbers1{};
    assert(std::abs(0 - proccess.ComputeDispersion(numbers1, 2)) < 1e-2);
    std::vector<double> numbers2{4};
    assert(std::abs(0 - proccess.ComputeDispersion(numbers2, 2)) < 1e-2);
}

void Tests::TestTakeInfoTimeAndMof()
{
    using namespace file;
    Proccess proccess;
    uint16_t size_window = 10800;
    proccess.SetTimeWindow(size_window);
    ReadFile read("test.txt");
    read.ReadMof();
    proccess.TakeInfoTimeAndMof();
    assert(proccess.mof_.time_per_sec_and_mof_[1].size() != 0); // Если стреляет данный ассерт, посмотри, сколько данных в файле - возможно нужно изменить размер временного окна (На самом деле, это символическая проверка)
}

void Tests::TestComputeTimeAndMiddleMof()
{
    using namespace file;
    Proccess proccess;
    uint16_t size_window = 10800;
    proccess.SetTimeWindow(size_window);
    ReadFile read("test.txt");
    read.ReadMof();
    proccess.ComputeTimeAndMiddleMof();
    assert(proccess.mof_.time_and_middle_mof_.size() != 0);
  //  assert(proccess.mof_.time_and_middle_mof_.size() == ) кол-во строк в файле <Если стреляет данный ассерт, посмотри, сколько данных в файле - возможно нужно изменить размер временного окна> так и есть в одно окно может несколько значений попадать, отсюда равенства не будет
}

void Tests::TestComputeTimeAndDispersionMof()
{
    using namespace file;
    Proccess proccess;
    uint16_t size_window = 10800;
    proccess.SetTimeWindow(size_window);
    ReadFile read("test.txt");
    read.ReadMof();
    proccess.ComputeTimeAndDispersionAndStandartDeviashionMof();
    assert(proccess.mof_.time_and_dispersion_.size() != 0);
//  assert(proccess.mof_.time_and_middle_mof_.size() == ) кол-во строк в файле <Если стреляет данный ассерт, посмотри, сколько данных в файле - возможно нужно изменить размер временного окна> так и есть в одно окно может несколько значений попадать, отсюда равенства не будет
}

void Tests::TestSumFiles()
{
    using namespace file;
    std::string name = "first.txt";
    std::string name1 = "second.txt";
    std::ofstream file_first(name);
    std::ofstream file_second(name1);
    file_first << "first" << std::endl <<"or no?" << std::endl;
    file_second << "second" << std::endl <<"or no?" << std::endl;
    assert(file_first.tellp() == 2);
    ReadFile read;
    read.SumFiles(name, name1);
    assert(file_first.tellp() == 4);
}

void Tests::TestAddFile()
{
    using namespace file;
    ReadFile read;
    std::string text = "start.txt";
    assert(read.files_.size() == 0);
    read.AddFile(text);
    assert(read.files_.size() == 1);
}

void Tests::TestReadMof()
{
    using namespace file;
    ReadFile read("test.txt");
    read.ReadMof();
    assert(read.file_.size() != 0);
}

void Tests::TestCalculateCountDataInWindowTime()
{
    using namespace file;
    ReadFile read("test.txt");
    read.ReadMof();
    Proccess proccess(read, 500);
    OutPutFile output(proccess);
    assert(output.count_points_in_time_window_.size() != 0);
}

void Tests::TestTransformTimeFromSeconds()
{
   using namespace file;
   OutPutFile outfile;
    assert(2 == outfile.TransformTimeFromSeconds(7200, 3600));
}
*/
