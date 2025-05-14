#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>

// Class representing a patient
class Patient {
private:
    std::string firstName;
    std::string lastName;
    int age;
    std::string region;
    std::string problem;

public:
    Patient() : firstName(""), lastName(""), age(0), region(""), problem("") {}
    Patient(const std::string& first, const std::string& last, int patientAge, const std::string& reg, const std::string& prob)
        : firstName(first), lastName(last), age(patientAge), region(reg), problem(prob) {}

    // Copy constructor
    Patient(const Patient& other)
        : firstName(other.firstName), lastName(other.lastName),
          age(other.age), region(other.region), problem(other.problem) {}

    // Destructor
    ~Patient() {}

    // Accessor functions
    std::string getFullName() const { return firstName + " " + lastName; }
    int getAge() const { return age; }
    std::string getRegion() const { return region; }
    std::string getProblem() const { return problem; }

    // Print patient details
    void printDetails() const {
        std::cout << "Patient: " << firstName << " " << lastName
                  << ", Age: " << age
                  << ", Region: " << region
                  << ", Problem: " << problem << std::endl;
    }
};

#endif // PATIENT_H