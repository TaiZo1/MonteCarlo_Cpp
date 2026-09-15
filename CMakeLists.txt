cmake_minimum_required(VERSION 3.20)
project(MonteCarlo_Cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tsp_demo src/main.cpp)
target_include_directories(tsp_demo PRIVATE include)

include(CTest)
if(BUILD_TESTING)
    add_executable(algorithm_tests tests/test_algorithms.cpp)
    target_include_directories(algorithm_tests PRIVATE include)
    add_test(NAME algorithms COMMAND algorithm_tests)
endif()

option(BUILD_LEGACY_EXAMPLES "Build the original statistical exercises" OFF)
if(BUILD_LEGACY_EXAMPLES)
    add_executable(simulations examples/legacy/src/simulations.cpp)
    target_include_directories(simulations PRIVATE examples/legacy/include)
endif()
