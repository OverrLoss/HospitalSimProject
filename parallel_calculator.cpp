#include "parallel_calculator.h"
#include <vector>
#include <random>
#include <omp.h>
#include <chrono>
#include <thread>
#include <cmath>

namespace {
    std::vector<double> data;
    int num_threads = 1;

    void update_threads_by_percent(int percent) {
        int hw_threads = std::thread::hardware_concurrency();
        if (hw_threads == 0) hw_threads = 2;
        int threads = (hw_threads * percent) / 100;
        if (threads < 1) threads = 1;
        num_threads = threads;
    }
}

extern "C" DLL_EXPORT void init_data(size_t n) {
    data.clear();
    data.resize(n);
    std::mt19937 rng(static_cast<unsigned>(std::random_device{}()));
    std::uniform_real_distribution<double> dist(0.0, 1000.0);

    for (size_t i = 0; i < n; ++i) {
        data[i] = dist(rng);
    }
}

extern "C" DLL_EXPORT void set_num_threads(int threads) {
    if (threads > 0)
        num_threads = threads;
}

extern "C" DLL_EXPORT void set_cpu_usage_percent(int percent) {
    if (percent > 0 && percent <= 100)
        update_threads_by_percent(percent);
}

extern "C" DLL_EXPORT double run_computation() {
    auto start = std::chrono::high_resolution_clock::now();

    double total = 0.0;
    size_t n = data.size();

    #pragma omp parallel for num_threads(num_threads) reduction(+:total) schedule(dynamic)
    for (ptrdiff_t i = 0; i < static_cast<ptrdiff_t>(n); ++i) {
        double x = data[i];
        double res = 1.0;
        for (int k = 1; k <= 100; ++k) {
            res += std::sin(x / (k+1)) * std::cos(x * k);
        }
        total += res;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    return elapsed;
}