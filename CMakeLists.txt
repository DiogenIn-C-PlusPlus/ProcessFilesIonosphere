cmake_minimum_required(VERSION 3.16)

project(Proc_Files_Ionosphere LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog CONFIG REQUIRED)
find_package(GTest REQUIRED)

add_library(core STATIC
    src/proccess.cpp src/buildhistogram.cpp src/additional.cpp
    src/cyprus.cpp src/alissprings.cpp src/algorithmskeleton.cpp src/measumentfactory.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(core PUBLIC spdlog::spdlog_header_only)

add_executable(Proc_Files_Ionosphere app/main.cpp)
target_link_libraries(Proc_Files_Ionosphere PRIVATE core)


add_executable(my_tests tests/tests.cpp)
target_link_libraries(my_tests PRIVATE core GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(my_tests)

include(GNUInstallDirs)
install(TARGETS Proc_Files_Ionosphere
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
