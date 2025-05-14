#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "doctor.h"
#include "patient.h"
#include <string>
#include <vector>
#include <iostream>

// Klasa reprezentująca szpital
class Hospital {
private:
    std::string name;
    std::string region;
    std::vector<Doctor*> doctors;
    std::vector<Patient*> assignedPatients;

public:
    // Konstruktor
    Hospital(const std::string& name, const std::string& region)
        : name(name), region(region) {}

    // Dodawanie lekarza
    void addDoctor(Doctor* doctor) {
        doctors.push_back(doctor);
    }

    // Zwracanie listy lekarzy
    const std::vector<Doctor*>& getDoctors() const {
        return doctors;
    }

    // Przypisywanie pacjentów do lekarzy
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

    // Wyświetlanie lekarzy
    void printDoctors() const {
        std::cout << "Doctors in " << name << " (" << region << "):\n";
        for (const Doctor* doctor : doctors) {
            std::cout << "Doctor: " << doctor->getFirstName() << " " << doctor->getLastName()
                      << ", Specialization: " << doctor->getSpecialization() << "\n";
        }
    }

    // Zwracanie nazwy szpitala
    const std::string& getName() const { return name; }

    // Zwracanie regionu
    const std::string& getRegion() const { return region; }
};

#endif // HOSPITAL_H