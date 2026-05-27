cmake_minimum_required(VERSION 3.15)
project(MonteCarlo_Cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(simulations src/simulations.cpp)
target_include_directories(simulations PRIVATE include)
