#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "parallel_calculator.h"

namespace py = pybind11;

PYBIND11_MODULE(parallel_calculator_py, m) {
    m.def("init_data", &init_data, "Initialize random data");
    m.def("set_num_threads", &set_num_threads, "Set number of threads");
    m.def("set_cpu_usage_percent", &set_cpu_usage_percent, "Set CPU usage percent");
    m.def("run_computation", &run_computation, "Run computation and return elapsed time");
}