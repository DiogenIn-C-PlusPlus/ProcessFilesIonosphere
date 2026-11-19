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
//    TestHistogram();
 //   TestMergePrognosticFreqAndExperimental();
}

void Tests::TestHistogram()
{
    // Создаем тестовые данные с явным кластером
    std::vector<double> test_data;

    // Основной кластер вокруг 15000-17900 (60% данных - 30 элементов)
    std::vector<double> expected_cluster;
    for (int i = 0; i < 30; ++i) {
        double val = 15000 + (i * 100); // 15000, 15100, ..., 17900
        test_data.push_back(val);
        expected_cluster.push_back(val);
    }

    // Разбросанные данные (40% данных - 20 элементов)
    std::vector<double> scattered_data = {
        10000, 11000, 12000, 13000, 14000,
        20000, 21000, 22000, 23000, 24000,
        25000, 26000, 27000, 28000, 29000,
        10500, 11500, 12500, 13500, 14500
    };
    test_data.insert(test_data.end(), scattered_data.begin(), scattered_data.end());

    std::cout << "=== TestHistogram ===" << std::endl;
    std::cout << "Input data size: " << test_data.size() << std::endl;
    std::cout << "Expected cluster (60%): 15000-17900 (30 elements)" << std::endl;
    std::cout << "Scattered data (40%): 20 elements outside cluster" << std::endl;

    // Создаем объект BuildHistogram с параметрами: step_mhz=1000, similary=0.6 (60%), start_size=3.0
    BuildHistogram builder(1000, 0.6, 3.0);

    // Вызываем тестируемую функцию
    auto result = builder.HistogramEveryWindow(test_data);

    std::cout << "\nResult size: " << result.size() << std::endl;
    std::cout << "Result values (sorted): ";
    std::vector<double> sorted_result = result;
    std::sort(sorted_result.begin(), sorted_result.end());
    for (double val : sorted_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Проверки
    bool test_passed = true;

    // 1. Проверяем, что результат не пустой
    if (result.empty()) {
        std::cout << "FAIL: Result is empty!" << std::endl;
        return;
    }

    // 2. Проверяем, что размер результата >= 60% от исходных данных
    size_t expected_min_size = static_cast<size_t>(test_data.size() * 0.6);
    if (result.size() < expected_min_size) {
        std::cout << "FAIL: Result size (" << result.size()
                  << ") is less than 60% of input data (" << expected_min_size << ")" << std::endl;
        test_passed = false;
    }

    // 3. Проверяем, что все элементы из ожидаемого кластера попали в результат
    std::vector<double> missing_from_cluster;
    for (double expected_val : expected_cluster) {
        bool found = false;
        for (double result_val : result) {
            if (std::abs(expected_val - result_val) < 1e-10) {
                found = true;
                break;
            }
        }
        if (!found) {
            missing_from_cluster.push_back(expected_val);
        }
    }

    if (!missing_from_cluster.empty()) {
        std::cout << "FAIL: Missing " << missing_from_cluster.size()
                  << " elements from expected cluster:" << std::endl;
        for (double val : missing_from_cluster) {
            std::cout << "  " << val << std::endl;
        }
        test_passed = false;
    } else {
        std::cout << "PASS: All 30 elements from expected cluster are in result" << std::endl;
    }

    // 4. Проверяем, что разбросанные данные НЕ попали в результат
    std::vector<double> incorrectly_included;
    for (double scattered_val : scattered_data) {
        bool found = false;
        for (double result_val : result) {
            if (std::abs(scattered_val - result_val) < 1e-10) {
                found = true;
                break;
            }
        }
        if (found) {
            incorrectly_included.push_back(scattered_val);
        }
    }

    if (!incorrectly_included.empty()) {
        std::cout << "WARNING: " << incorrectly_included.size()
                  << " scattered elements included in result:" << std::endl;
        for (double val : incorrectly_included) {
            std::cout << "  " << val << std::endl;
        }
        // Это не обязательно ошибка, если размер окна позволяет включить несколько лишних элементов
        if (incorrectly_included.size() > 5) {
            std::cout << "FAIL: Too many scattered elements included" << std::endl;
            test_passed = false;
        }
    } else {
        std::cout << "PASS: No scattered elements included in result" << std::endl;
    }

    // 5. Проверяем, что диапазон результата соответствует кластеру
    double result_min = *std::min_element(result.begin(), result.end());
    double result_max = *std::max_element(result.begin(), result.end());

    std::cout << "\nResult range: [" << result_min << ", " << result_max << "]" << std::endl;
    std::cout << "Expected cluster range: [15000, 17900]" << std::endl;

    // Проверяем, что большинство элементов в диапазоне кластера
    size_t elements_in_cluster_range = 0;
    for (double val : result) {
        if (val >= 15000 && val <= 17900) {
            elements_in_cluster_range++;
        }
    }

    double percentage_in_cluster = (static_cast<double>(elements_in_cluster_range) / result.size()) * 100;
    std::cout << "Elements in cluster range: " << elements_in_cluster_range
              << " out of " << result.size() << " (" << percentage_in_cluster << "%)" << std::endl;

    if (percentage_in_cluster < 80) {
        std::cout << "FAIL: Less than 80% of result elements are in cluster range" << std::endl;
        test_passed = false;
    } else {
        std::cout << "PASS: " << percentage_in_cluster << "% of result elements are in cluster range" << std::endl;
    }

    // Итоговый результат
    if (test_passed) {
        std::cout << "\n*** TEST PASSED ***" << std::endl;
        std::cout << "Function correctly identified the dense cluster of 60% data" << std::endl;
    } else {
        std::cout << "\n*** TEST FAILED ***" << std::endl;
    }

    std::cout << "=== End TestHistogram ===" << std::endl << std::endl;
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



//    std::vector<double> delta_window_between;
//    std::map<size_t, size_t> number_hop_count_modes;
//    size_t all_modes = 0;
//    size_t current_number_hop = 0;
//    for(const auto& [time, number_hop_freq] : prognostic_time_and_number_hops_freq_)
//    {
//        for(size_t i = 1;  i < number_hop_freq.size(); ++i)
//        {
//            delta_window_between.push_back(std::abs(number_hop_freq[i - 1].freq - number_hop_freq[i].freq));
//        }
//        std::sort(filtered_mof.begin(), filtered_mof.end(), [](double lhs, double rhs)
//        {
//            return rhs < lhs;
//        });
//        double point_start_window = filtered_mof[current_number_hop]; // изначальная позиция для данного временного окна
//        for(size_t i = 0, number_window = 0; i < filtered_mof.size(); ++i)
//        {
//            double current_value_freq = filtered_mof[i];
//            double shift_current_freq_about_delta_window = point_start_window - current_value_freq;
//            double prognostic_delta_between_window = 0;
//            if(number_window >= delta_window_between.size())
//            {
//                ++number_hop_count_modes[number_hop_freq[++current_number_hop].hop];
//                continue;
//            }
//            else
//            {
//                prognostic_delta_between_window = delta_window_between[number_window];
//            }
//            if(shift_current_freq_about_delta_window < prognostic_delta_between_window)
//            {
//                ++number_hop_count_modes[number_hop_freq[current_number_hop++].hop]; // текущее и смещаем на следующее
//            }
//            else
//            {
//                if(i == number_hop_freq.size() - 1) // Последнее просто добавляем и выходим проверка на пустоту выше находится
//                {
//                    ++number_hop_count_modes[number_hop_freq[++current_number_hop].hop];
//                    ++all_modes;
//                    break;
//                }
//                if(shift_current_freq_about_delta_window < delta_window_between[number_window] + delta_window_between[number_window + 1]) // вопрос границ
//                {
//                    point_start_window -= delta_window_between[number_window];
//                    ++number_window;
//                    ++number_hop_count_modes[number_hop_freq[++current_number_hop].hop]; // берем сразу следующее так как она из следующего окна
//                }
//                else
//                {
//                    double sum_shift_delta_window = 0;
//                    for(size_t i = number_window; i < delta_window_between.size(); ++i, ++current_number_hop)
//                    {
//                        sum_shift_delta_window += delta_window_between[i];
//                        if(sum_shift_delta_window > shift_current_freq_about_delta_window)
//                        {
//                            sum_shift_delta_window -= delta_window_between[i];
//                            point_start_window -= sum_shift_delta_window;
//                            ++number_hop_count_modes[number_hop_freq[--current_number_hop].hop]; // Не понятно почему нужен --, мб просто без него
//                            number_window = i - 1; // здесь проблемы нет по поводу границ изначально уже будет на 1 больше исходя из предыдущего условия
//                            break;
//                        }
//                    }
//                }
//            }
//        ++all_modes;
//        }
//    }
//}
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
