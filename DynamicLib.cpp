#include "DynamicLib.h"
#include <omp.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

DynamicLib::DynamicLib() : numThreads(omp_get_max_threads()) {}

DynamicLib::~DynamicLib() {}

void DynamicLib::setThreadUsagePercentage(int percentage) {
    if (percentage < 1 || percentage > 100) {
        throw std::invalid_argument("Percentage must be between 1 and 100.");
    }
    numThreads = (omp_get_max_threads() * percentage) / 100;
    if (numThreads < 1) numThreads = 1;
    omp_set_num_threads(numThreads);
}

std::vector<int> DynamicLib::generateRandomData(size_t size) {
    std::vector<int> data(size);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);

#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        data[i] = dist(rng);
    }
    return data;
}

int DynamicLib::parallelSum(const std::vector<int>& data) {
    int sum = 0;

#pragma omp parallel for reduction(+ : sum)
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return sum;
}

double DynamicLib::measureExecutionTime(size_t dataSize) {
    auto start = std::chrono::high_resolution_clock::now();

    // Generate random data and compute parallel sum
    auto data = generateRandomData(dataSize);
    int sum = parallelSum(data);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Print result for reference
    std::cout << "Parallel sum: " << sum << std::endl;
    return elapsed.count();
}