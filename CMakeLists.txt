cmake_minimum_required(VERSION 3.15)
project(HospitalSimProject LANGUAGES CXX)

# Główna aplikacja konsolowa (z wszystkimi plikami)
add_executable(hospital_sim
    main.cpp
    parallel_calculator.cpp
)

target_include_directories(hospital_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Users/xhero/AppData/Local/Packages/PythonSoftwareFoundation.Python.3.13_qbz5n2kfra8p0/LocalCache/local-packages/Python313/site-packages/pybind11/share/cmake/pybind11")
# OpenMP (opcjonalnie, jeśli chcesz używać wielowątkowości)
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(hospital_sim PRIVATE OpenMP::OpenMP_CXX)
endif()

# Budowanie modułu Python (pybind11)
find_package(pybind11 REQUIRED)
add_library(parallel_calculator_py MODULE python_bindings.cpp parallel_calculator.cpp)
target_include_directories(parallel_calculator_py PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(parallel_calculator_py PRIVATE pybind11::module)
if(OpenMP_CXX_FOUND)
    target_link_libraries(parallel_calculator_py PRIVATE OpenMP::OpenMP_CXX)
endif()

set_target_properties(parallel_calculator_py PROPERTIES
    PREFIX ""         # Dla importu w Pythonie: nie dodawaj "lib" przed nazwą!
    SUFFIX ".pyd"     # Rozszerzenie modułu dla Windows
)

# Opcjonalnie: wymuś standard C++17 jeśli domyślny nie jest wystarczający
set_property(TARGET hospital_sim parallel_calculator_py PROPERTY CXX_STANDARD 17)