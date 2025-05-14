#ifndef DOCTOR_H
#define DOCTOR_H

#include "patient.h"
#include <string>
#include <vector>
#include <iostream>

// Klasa reprezentująca lekarza
class Doctor {
private:
    std::string firstName;
    std::string lastName;
    std::string specialization;
    std::vector<Patient*> patients; // Lista pacjentów przypisanych do lekarza

public:
    // Konstruktor
    Doctor(const std::string& firstName, const std::string& lastName, const std::string& specialization)
        : firstName(firstName), lastName(lastName), specialization(specialization) {}

    // Pobranie imienia lekarza
    const std::string& getFirstName() const {
        return firstName;
    }

    // Pobranie nazwiska lekarza
    const std::string& getLastName() const {
        return lastName;
    }

    // Pobranie specjalizacji lekarza
    const std::string& getSpecialization() const {
        return specialization;
    }

    // Dodanie pacjenta do listy pacjentów lekarza
    void addPatient(Patient* patient) {
        patients.push_back(patient);
    }

    // Wyświetlenie wszystkich pacjentów lekarza
    void printPatients() const {
        std::cout << "Patients of Dr. " << firstName << " " << lastName << " (" << specialization << "):\n";
        for (const Patient* patient : patients) {
            std::cout << " - " << patient->getFirstName() << " " << patient->getLastName() << ", Age: " << patient->getAge()
                      << ", Problem: " << patient->getProblem() << "\n";
        }
    }
};

#endif // DOCTOR_H