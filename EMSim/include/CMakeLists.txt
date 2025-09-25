cmake_minimum_required(VERSION 3.20)
# # cmake -S . -B build -G "MinGW Makefiles"
# set(CMAKE_CXX_COMPILER "C:/ProgramData/mingw64/mingw64/bin/g++.exe")
# set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")

project(EMSim)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")

set(INC_FILES *.h)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

