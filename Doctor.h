#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <vector>
#include <iostream>

class Doctor {
private:
    std::string name;
    std::string specialization;

public:
    Doctor(const std::string& name, const std::string& specialization);
    std::string getSpecialization() const;
    void print() const;
};

#endif