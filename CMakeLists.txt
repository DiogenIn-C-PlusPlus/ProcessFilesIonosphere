cmake_minimum_required(VERSION 3.16)

project(Proc_Files_Ionosphere LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Proc_Files_Ionosphere main.cpp
    outputfile.cpp outputfile.h proccess.cpp proccess.h readfile.cpp readfile.h)

include(GNUInstallDirs)
install(TARGETS Proc_Files_Ionosphere
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
