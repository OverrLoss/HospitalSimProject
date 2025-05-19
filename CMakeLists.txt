cmake_minimum_required(VERSION 3.15)
project(parallel_calculator_py LANGUAGES CXX)

find_package(pybind11 REQUIRED)
find_package(OpenMP REQUIRED)

add_library(parallel_calculator_py MODULE python_bindings.cpp parallel_calculator.cpp)
target_include_directories(parallel_calculator_py PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parallel_calculator_py PRIVATE pybind11::module OpenMP::OpenMP_CXX)

set_target_properties(parallel_calculator_py PROPERTIES
    PREFIX ""  # Ważne dla modułów pythonowych!
    SUFFIX