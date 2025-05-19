#ifndef PATIENT_H
#define PATIENT_H

#include "person.h"
#include <string>
#include <iostream>

class Patient : public Person {
private:
    int age;
    std::string region;
    std::string problem;
    std::string visitDateTime; // Data i godzina wizyty

public:
    Patient() : Person(), age(0), region(""), problem("") {}
    Patient(const std::string& firstName, const std::string& lastName, int age, const std::string& region, const std::string& problem)
        : Person(firstName, lastName), age(age), region(region), problem(problem) {}
    Patient(const Patient& other)
        : Person(other), age(other.age), region(other.region), problem(other.problem), visitDateTime(other.visitDateTime) {}
    ~Patient() override {}

    int getAge() const { return age; }
    const std::string& getRegion() const { return region; }
    const std::string& getProblem() const { return problem; }

    void setVisitDateTime(const std::string& dt) { visitDateTime = dt; }
    const std::string& getVisitDateTime() const { return visitDateTime; }

    void printDetails() const override {
        std::cout << "Patient: " << firstName << " " << lastName
                  << ", Age: " << age
                  << ", Region: " << region
                  << ", Problem: " << problem
                  << (visitDateTime.empty() ? "" : (", Visit: " + visitDateTime))
                  << std::endl;
    }
};

#endif // PATIENT_H