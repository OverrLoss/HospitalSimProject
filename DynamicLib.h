#ifndef DYNAMIC_LIB_H
#define DYNAMIC_LIB_H

#include <vector>
#include <cstddef>

class DynamicLib {
private:
    int numThreads;

public:
    DynamicLib();
    ~DynamicLib();

    // Set percentage of CPU resources to be used (1-100%)
    void setThreadUsagePercentage(int percentage);

    // Generate random data of a given size
    std::vector<int> generateRandomData(size_t size);

    // Compute parallel sum of the data
    int parallelSum(const std::vector<int>& data);

    // Measure execution time for generating data and computing sum
    double measureExecutionTime(size_t dataSize);
};

#endif