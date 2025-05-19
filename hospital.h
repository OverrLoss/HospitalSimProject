#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "doctor.h"
#include "patient.h"
#include <string>
#include <vector>
#include <iostream>

class Hospital {
private:
    std::string name;
    std::string region;
    std::vector<Doctor*> doctors;
    std::vector<Patient*> assignedPatients;

public:
    Hospital(const std::string& name, const std::string& region)
        : name(name), region(region) {}

    void addDoctor(Doctor* doctor) { doctors.push_back(doctor); }
    const std::vector<Doctor*>& getDoctors() const { return doctors; }
    const std::string& getName() const { return name; }
    const std::string& getRegion() const { return region; }

    void assignPatientsToDoctors() {
        if (doctors.empty()) {
            std::cerr << "No doctors available to assign patients.\n";
            return;
        }
        size_t doctorIndex = 0;
        for (Patient* patient : assignedPatients) {
            doctors[doctorIndex]->addPatient(patient);
            doctorIndex = (doctorIndex + 1) % doctors.size(); // Round-robin assignment
        }
        std::cout << "Patients assigned to doctors successfully.\n";
    }

    void printDoctors() const {
        std::cout << "Doctors in " << name << " (" << region << "):\n";
        for (const Doctor* doctor : doctors) {
            std::cout << "Doctor: " << doctor->getFirstName() << " " << doctor->getLastName()
                      << ", Specialization: " << doctor->getSpecialization() << "\n";
        }
    }
};

#endif // HOSPITAL_H