#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "doctor.h"
#include "patient.h"
#include <vector>
#include <string>
#include <iostream>

class Hospital {
private:
    std::string name;                // Nazwa szpitala
    std::string region;              // Województwo
    std::vector<Doctor*> doctors;    // Lista lekarzy
    std::vector<Patient*> patients;  // Lista pacjentów

public:
    // Konstruktor domyślny
    Hospital() : name(""), region("") {}

    // Konstruktor z parametrami
    Hospital(const std::string& hospitalName, const std::string& hospitalRegion)
        : name(hospitalName), region(hospitalRegion) {}

    // Destruktor - zwalnia pamięć dynamicznie alokowaną
    ~Hospital() {
        for (Doctor* doctor : doctors) {
            delete doctor;
        }
        for (Patient* patient : patients) {
            delete patient;
        }
    }

    // Dodawanie lekarzy
    void addDoctor(const std::string& firstName, const std::string& lastName, const std::string& specialization) {
        Doctor* newDoctor = new Doctor(firstName, lastName, specialization, name, region);
        doctors.push_back(newDoctor);
    }

    // Dodawanie pacjentów
    void addPatient(const std::string& firstName, const std::string& lastName, int age, const std::string& problem) {
        Patient* newPatient = new Patient(firstName, lastName, age, region, problem);
        patients.push_back(newPatient);
    }

    // Wyświetlanie wszystkich lekarzy
    void printDoctors() const {
        std::cout << "Doctors in " << name << " (" << region << "):" << std::endl;
        for (const Doctor* doctor : doctors) {
            doctor->printDetails();
        }
    }

    // Wyświetlanie wszystkich pacjentów
    void printPatients() const {
        std::cout << "Patients in " << name << " (" << region << "):" << std::endl;
        for (const Patient* patient : patients) {
            patient->printDetails();
        }
    }

    // Pobieranie regionu
    std::string getRegion() const {
        return region;
    }

    // Funkcja przypisująca pacjenta do odpowiedniego lekarza w szpitalu
    void assignPatientsToDoctors() {
        for (Patient* patient : patients) {
            bool assigned = false;
            for (Doctor* doctor : doctors) {
                if (doctor->getSpecialization() == patient->getProblem()) {
                    std::cout << "Assigning " << patient->getFullName() << " to Dr. "
                              << doctor->getSpecialization() << std::endl;
                    assigned = true;
                    break;
                }
            }
            if (!assigned) {
                std::cout << "Assigning " << patient->getFullName()
                          << " to a general practitioner." << std::endl;
            }
        }
    }
};

#endif // HOSPITAL_H