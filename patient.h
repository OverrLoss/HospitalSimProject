#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>

class Patient {
private:
    std::string firstName;
    std::string lastName;
    int age;
    std::string region;
    std::string problem;
    std::string visitDateTime; // Data i godzina wizyty

public:
    Patient(const std::string& firstName, const std::string& lastName, int age, const std::string& region, const std::string& problem)
        : firstName(firstName), lastName(lastName), age(age), region(region), problem(problem) {}

    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    int getAge() const { return age; }
    const std::string& getRegion() const { return region; }
    const std::string& getProblem() const { return problem; }

    void setVisitDateTime(const std::string& dt) { visitDateTime = dt; }
    const std::string& getVisitDateTime() const { return visitDateTime; }

    void printDetails() const {
        std::cout << "Patient: " << firstName << " " << lastName
                  << ", Age: " << age
                  << ", Region: " << region
                  << ", Problem: " << problem
                  << (visitDateTime.empty() ? "" : (", Visit: " + visitDateTime))
                  << std::endl;
    }
};

#endif // PATIENT_H