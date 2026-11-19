cmake_minimum_required(VERSION 3.16)

project(Proc_Files_Ionosphere LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog CONFIG REQUIRED)
find_package(GTest REQUIRED)

add_executable(Proc_Files_Ionosphere
    main.cpp
    proccess.cpp proccess.h
    buildhistogram.h buildhistogram.cpp
    formatfiles.h
    tests.h tests.cpp
    additional.h additional.cpp
    cyprus.h cyprus.cpp
    alissprings.h alissprings.cpp
    algorithmskeleton.h algorithmskeleton.cpp
    measumentfactory.h measumentfactory.cpp
    magic_enum.hpp
)

target_link_libraries(Proc_Files_Ionosphere PRIVATE spdlog::spdlog_header_only)

add_executable(my_tests
    tests.cpp
    additional.cpp
    proccess.cpp
    buildhistogram.cpp
    cyprus.cpp
    alissprings.cpp
    algorithmskeleton.cpp
)

target_link_libraries(my_tests PRIVATE
    GTest::gtest_main
    spdlog::spdlog_header_only
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(my_tests)

include(GNUInstallDirs)
install(TARGETS Proc_Files_Ionosphere
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
