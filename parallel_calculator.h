#ifndef PARALLEL_CALCULATOR_H
#define PARALLEL_CALCULATOR_H

#include <cstddef>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C" {
    DLL_EXPORT void init_data(size_t n);
    DLL_EXPORT void set_num_threads(int num_threads);
    DLL_EXPORT void set_cpu_usage_percent(int percent);
    DLL_EXPORT double run_computation(); // Zwraca czas w sekundach
}

#endif // PARALLEL_CALCULATOR_H