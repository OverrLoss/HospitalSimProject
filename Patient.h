#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>

class Patient {
private:
    std::string name;
    int age;
    std::string region;
    std::string problem;

public:
    Patient(const std::string& name, int age, const std::string& region, const std::string& problem);
    std::string getRegion() const;
    std::string getProblem() const;
    void print() const;
};

#endif