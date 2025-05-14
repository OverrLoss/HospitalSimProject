#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <iostream>

// Abstract class representing a generic medical professional
class MedicalProfessional {
protected:
    std::string firstName;
    std::string lastName;
    std::string specialization;

public:
    MedicalProfessional() : firstName(""), lastName(""), specialization("") {}
    MedicalProfessional(const std::string& first, const std::string& last, const std::string& spec)
        : firstName(first), lastName(last), specialization(spec) {}
    virtual ~MedicalProfessional() {}

    // Virtual function to print details
    virtual void printDetails() const = 0;
};

// Class representing a doctor
class Doctor : public MedicalProfessional {
private:
    std::string hospitalName;
    std::string region;

public:
    Doctor() : hospitalName(""), region("") {}
    Doctor(const std::string& first, const std::string& last, const std::string& spec, const std::string& hospital, const std::string& reg)
        : MedicalProfessional(first, last, spec), hospitalName(hospital), region(reg) {}

    // Copy constructor
    Doctor(const Doctor& other)
        : MedicalProfessional(other.firstName, other.lastName, other.specialization),
          hospitalName(other.hospitalName), region(other.region) {}

    // Destructor
    ~Doctor() {}

    // Accessor functions
    std::string getHospitalName() const { return hospitalName; }
    std::string getRegion() const { return region; }

    // Getter for specialization
    std::string getSpecialization() const { return specialization; }

    // Override printDetails
    void printDetails() const override {
        std::cout << "Doctor: " << firstName << " " << lastName
                  << ", Specialization: " << specialization
                  << ", Hospital: " << hospitalName
                  << ", Region: " << region << std::endl;
    }
};

#endif // DOCTOR_H