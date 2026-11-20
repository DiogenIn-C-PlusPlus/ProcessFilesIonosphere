#include <gtest/gtest.h>

#include "proccess.h"
#include "buildhistogram.h"


TEST(HistogramTest, EveryWindow)
{
    // --- Подготовка данных ---
    std::vector<double> test_data;
    std::vector<double> expected_cluster;
    for (int i = 0; i < 30; ++i) {
        double val = 15000 + (i * 150);
        test_data.push_back(val);
        expected_cluster.push_back(val);
    }
    std::vector<double> scattered_data = {
        10000, 11000, 12000, 13000, 14000,
        20000, 21000, 22000, 23000, 24000,
        25000, 26000, 27000, 28000, 29000,
        10500, 11500, 12500, 13500, 14500
    };
    test_data.insert(test_data.end(), scattered_data.begin(), scattered_data.end());

    // --- Действие ---
    BuildHistogram builder(1000, 0.6, 3.0);
    auto result = builder.HistogramEveryWindow(test_data);

    // --- Проверки ---
    ASSERT_FALSE(result.empty()) << "Result is empty";

    // размер >= 60% от исходных данных
    size_t expected_min_size = static_cast<size_t>(test_data.size() * 0.6);
    EXPECT_GE(result.size(), expected_min_size);

    // все элементы кластера присутствуют
    for (double expected_val : expected_cluster) {
        bool found = std::any_of(result.begin(), result.end(),
            [expected_val](double v) { return std::abs(expected_val - v) < 1e-10; });
        EXPECT_TRUE(found) << "Missing cluster element: " << expected_val;
    }

    // большинство элементов в диапазоне кластера
    size_t in_range = std::count_if(result.begin(), result.end(),
        [](double v) { return v >= 15000 && v <= 17900; });
    double percentage = 100.0 * in_range / result.size();
    EXPECT_GE(percentage, 80.0) << "Only " << percentage << "% in cluster range"; // означает, что захватил искомый диапазон более 80%
}

TEST(HistogramTest, MaxWindow)
{
    // --- Подготовка данных ---
    std::vector<double> test_data;
    std::vector<double> expected_cluster;
    for (int i = 0; i < 30; ++i) {
        double val = 15000 + (i * 50);
        test_data.push_back(val);
        expected_cluster.push_back(val);
    }
    std::vector<double> scattered_data =
    {
        10000, 11000, 12000, 13000, 14000,
        20000, 21000, 22000, 23000, 24000,
        25000, 26000, 27000, 28000, 29000,
        10500, 11500, 12500, 13500, 14500
    };
    test_data.insert(test_data.end(), scattered_data.begin(), scattered_data.end());

    // --- Действие ---
    BuildHistogram builder(1000, 0.6, 0.5);
    auto result = builder.HistogramMaxWindow(test_data);

    // --- Проверки ---
    ASSERT_FALSE(result.empty()) << "Result is empty";

    // размер >= 60% от исходных данных
    size_t expected_min_size = static_cast<size_t>(test_data.size() * 0.6);
    EXPECT_GE(result.size(), expected_min_size);

    // все элементы кластера присутствуют
    for (double expected_val : expected_cluster) {
        bool found = std::any_of(result.begin(), result.end(),
            [expected_val](double v) { return std::abs(expected_val - v) < 1e-10; });
        EXPECT_TRUE(found) << "Missing cluster element: " << expected_val;
    }

    // большинство элементов в диапазоне кластера
    size_t in_range = std::count_if(result.begin(), result.end(),
        [](double v) { return v >= 15000 && v <= 17900; });
    double percentage = 100.0 * in_range / result.size();
    EXPECT_GE(percentage, 100) << "Only " << percentage << "% in cluster range"; // означает, что захватил искомый диапазон более 80%
}

TEST(HistogramTest, ProbilityWindow)
{
    // --- Подготовка данных ---
    std::vector<double> expected_cluster =
    {
        400.0,      // [9500, 10500)   10000 * 1/25
        860.0,      // [10500, 11500)  10750 * 2/25
        940.0,      // [11500, 12500)  11750 * 2/25
        1020.0,     // [12500, 13500)  12750 * 2/25
        1100.0,     // [13500, 14500)  13750 * 2/25
        580.0,      // [14500, 15500)  14500 * 1/25
        800.0,      // [19500, 20500)  20000 * 1/25
        840.0,      // [20500, 21500)  21000 * 1/25
        880.0,      // [21500, 22500)  22000 * 1/25
        920.0,      // [22500, 23500)  23000 * 1/25
        960.0,      // [23500, 24500)  24000 * 1/25
        1000.0,     // [24500, 25500)  25000 * 1/25
        1040.0,     // [25500, 26500)  26000 * 1/25
        1080.0,     // [26500, 27500)  27000 * 1/25
        1120.0,     // [27500, 28500)  28000 * 1/25
        1160.0,     // [28500, 29500)  29000 * 1/25
        3741.2,     // [30500, 31500)  31176.666... * 3/25
        2584.0      // [31500, 32500)  32300 * 2/25
    };

    std::vector<double> scattered_data =
    {
        10000, 11000, 12000, 13000, 14000,
        20000, 21000, 22000, 23000, 24000,
        25000, 26000, 27000, 28000, 29000,
        10500, 11500, 12500, 13500, 14500,
        31150, 31170, 31210, 32200, 32400
    };

    // --- Действие ---
    BuildHistogram builder(1000);
    auto result = builder.HistogramProbilityWindow(scattered_data);

    // --- Проверки ---
    ASSERT_FALSE(result.empty()) << "Result is empty";

    // размер 100% от исходных данных
    size_t expected_min_size = static_cast<size_t>(scattered_data.size());
    EXPECT_GE(result.size(), expected_min_size);

    // все элементы кластера присутствуют
    for (double expected_val : expected_cluster) {
        bool found = std::any_of(result.begin(), result.end(),
            [expected_val](double v) { return std::abs(expected_val - v) < 1e-10; });
        EXPECT_TRUE(found) << "Missing cluster element: " << expected_val;
    }
}

TEST(ProccessTest, MiddleValue)
{
    Proccess prosses;
    EXPECT_NEAR(prosses.MiddleValue({1,2,3,4,5,6,7,8,9,10}), 5.5, 1e-5);
    EXPECT_NEAR(prosses.MiddleValue({}), 0.0, 1e-5);
    EXPECT_NEAR(prosses.MiddleValue({2}), 2.0, 1e-5);
}

TEST(ProccessTest, Dispersion)
{
    Proccess proccess;
    std::vector<double> numbers = {12,23,45,33,65,54,54};
    double middle = proccess.MiddleValue(numbers);
    EXPECT_NEAR(middle, 40.857, 1e-3);
    EXPECT_NEAR(proccess.ComputeDispersion(numbers, middle), 359.81, 1e-2);
    EXPECT_NEAR(proccess.ComputeDispersion({}, 0), 0.0, 1e-5);
}

TEST(ProccessTest, StandartDeviation)
{
    Proccess proccess;
    std::vector<double> numbers = {12,23,45,33,65,54,54};
    double middle = proccess.MiddleValue(numbers);
    double dispersion = proccess.ComputeDispersion(numbers, middle);
    EXPECT_NEAR(proccess.ComputeStandartDeviation(dispersion), std::sqrt(359.81), 1e-2);
    EXPECT_NEAR(proccess.ComputeStandartDeviation(-1), 0.0, 1e-5);
}

TEST(ProccessTest, ChangeMoscowTimeToUt)
{
    Proccess proccess;
    EXPECT_EQ(proccess.ChangeMoscowTimeToUtInSeconds(7000),  17800u);
    EXPECT_EQ(proccess.ChangeMoscowTimeToUtInSeconds(82800), 7200u);
}

TEST(ProccessTest, TransformTimeFromSeconds)
{
    Proccess proc;
    EXPECT_EQ(proc.TransformTimeFromSeconds(12, 900), 3.125);
}

//void Tests::TestMergePrognosticFreqAndExperimental()
//{
//    std::vector<double> filtered_mof =
//    {
//        10234, 11567, 12450, 13890, 14567,
//        15890, 16234, 17456, 18345, 19012,
//        20123, 21456, 22345, 23123, 24567,
//        25432, 26123, 27345, 28456, 29123,
//        30234, 31234, 32145, 33012, 34123,
//        10987, 12987, 14987, 16987, 18987,
//        20987, 22987, 24987, 26987, 28987,
//        30987, 32987, 33987, 34765, 34999
//    };



//void Tests::TestTakeInfoTimeAndMof()
//{
//    using namespace file;
//    Proccess proccess;
//    uint16_t size_window = 10800;
//    proccess.SetTimeWindow(size_window);
//    ReadFile read("test.txt");
//    read.ReadMof();
//    proccess.TakeInfoTimeAndMof();
//    assert(proccess.mof_.time_per_sec_and_mof_[1].size() != 0); // Если стреляет данный ассерт, посмотри, сколько данных в файле - возможно нужно изменить размер временного окна (На самом деле, это символическая проверка)
//}

//void Tests::TestComputeTimeAndMiddleMof()
//{
//    using namespace file;
//    Proccess proccess;
//    uint16_t size_window = 10800;
//    proccess.SetTimeWindow(size_window);
//    ReadFile read("test.txt");
//    read.ReadMof();
//    proccess.ComputeTimeAndMiddleMof();
//    assert(proccess.mof_.time_and_middle_mof_.size() != 0);
//  //  assert(proccess.mof_.time_and_middle_mof_.size() == ) кол-во строк в файле <Если стреляет данный ассерт, посмотри, сколько данных в файле - возможно нужно изменить размер временного окна> так и есть в одно окно может несколько значений попадать, отсюда равенства не будет
//}

//void Tests::TestComputeTimeAndDispersionMof()
//{
//    using namespace file;
//    Proccess proccess;
//    uint16_t size_window = 10800;
//    proccess.SetTimeWindow(size_window);
//    ReadFile read("test.txt");
//    read.ReadMof();
//    proccess.ComputeTimeAndDispersionAndStandartDeviashionMof();
//    assert(proccess.mof_.time_and_dispersion_.size() != 0);
////  assert(proccess.mof_.time_and_middle_mof_.size() == ) кол-во строк в файле <Если стреляет данный ассерт, посмотри, сколько данных в файле - возможно нужно изменить размер временного окна> так и есть в одно окно может несколько значений попадать, отсюда равенства не будет
//}

//void Tests::TestSumFiles()
//{
//    using namespace file;
//    std::string name = "first.txt";
//    std::string name1 = "second.txt";
//    std::ofstream file_first(name);
//    std::ofstream file_second(name1);
//    file_first << "first" << std::endl <<"or no?" << std::endl;
//    file_second << "second" << std::endl <<"or no?" << std::endl;
//    assert(file_first.tellp() == 2);
//    ReadFile read;
//    read.SumFiles(name, name1);
//    assert(file_first.tellp() == 4);
//}

//void Tests::TestAddFile()
//{
//    using namespace file;
//    ReadFile read;
//    std::string text = "start.txt";
//    assert(read.files_.size() == 0);
//    read.AddFile(text);
//    assert(read.files_.size() == 1);
//}

//void Tests::TestReadMof()
//{
//    using namespace file;
//    ReadFile read("test.txt");
//    read.ReadMof();
//    assert(read.file_.size() != 0);
//}

//void Tests::TestCalculateCountDataInWindowTime()
//{
//    using namespace file;
//    ReadFile read("test.txt");
//    read.ReadMof();
//    Proccess proccess(read, 500);
//    OutPutFile output(proccess);
//    assert(output.count_points_in_time_window_.size() != 0);
//}

//void Tests::TestTransformTimeFromSeconds()
//{
//   using namespace file;
//   OutPutFile outfile;
//    assert(2 == outfile.TransformTimeFromSeconds(7200, 3600));
//}
