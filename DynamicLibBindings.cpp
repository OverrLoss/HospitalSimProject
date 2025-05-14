#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "DynamicLib.h"

namespace py = pybind11;

PYBIND11_MODULE(DynamicLib, m) {
    py::class_<DynamicLib>(m, "DynamicLib")
        .def(py::init<>())
        .def("setThreadUsagePercentage", &DynamicLib::setThreadUsagePercentage, "Set percentage of CPU resources to use")
        .def("generateRandomData", &DynamicLib::generateRandomData, "Generate random data of given size")
        .def("parallelSum", &DynamicLib::parallelSum, "Compute parallel sum of the data")
        .def("measureExecutionTime", &DynamicLib::measureExecutionTime, "Measure execution time for operations");
}