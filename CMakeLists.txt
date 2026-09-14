cmake_minimum_required(VERSION 3.16)

project(Proc_Files_Ionosphere LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Proc_Files_Ionosphere main.cpp
    proccess.cpp proccess.h buildhistogram.h buildhistogram.cpp formatfiles.h tests.h tests.cpp additional.h additional.cpp cyprus.h cyprus.cpp
    alissprings.h alissprings.cpp algorithmskeleton.h algorithmskeleton.cpp measumentfactory.h measumentfactory.cpp
    additional.cpp additional.h algorithmskeleton.cpp algorithmskeleton.h alissprings.cpp alissprings.h buildhistogram.cpp buildhistogram.h magic_enum.hpp )

include(GNUInstallDirs)
install(TARGETS Proc_Files_Ionosphere
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
