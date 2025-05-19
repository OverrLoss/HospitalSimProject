#ifndef DOCTOR_H
#define DOCTOR_H

#include "patient.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

class Doctor {
private:
    std::string firstName;
    std::string lastName;
    std::string specialization;
    std::vector<Patient*> patients; // Lista pacjentów przypisanych do lekarza
    std::queue<Patient*> patientQueue; // Kolejka pacjentów do lekarza

public:
    Doctor(const std::string& firstName, const std::string& lastName, const std::string& specialization)
        : firstName(firstName), lastName(lastName), specialization(specialization) {}

    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    const std::string& getSpecialization() const { return specialization; }

    void addPatient(Patient* patient) {
        patients.push_back(patient);
        patientQueue.push(patient);
    }

    int getPatientsCount() const { return patients.size(); }
    const std::vector<Patient*>& getPatients() const { return patients; }

    void printPatients() const {
        std::cout << "Patients of Dr. " << firstName << " " << lastName << " (" << specialization << "):\n";
        for (const Patient* patient : patients) {
            std::cout << " - " << patient->getFirstName() << " " << patient->getLastName()
                      << ", Age: " << patient->getAge()
                      << ", Problem: " << patient->getProblem();
            if (!patient->getVisitDateTime().empty())
                std::cout << ", Visit: " << patient->getVisitDateTime();
            std::cout << "\n";
        }
    }

    void printQueue() const {
        std::cout << "Queue for Dr. " << firstName << " " << lastName << " (" << specialization << "):\n";
        if (patientQueue.empty()) {
            std::cout << "  No patients in the queue.\n";
            return;
        }
        std::queue<Patient*> copy = patientQueue;
        int pos = 1;
        while (!copy.empty()) {
            const Patient* patient = copy.front();
            std::cout << " " << pos << ". " << patient->getFirstName() << " " << patient->getLastName()
                      << ", Age: " << patient->getAge() << ", Problem: " << patient->getProblem();
            if (!patient->getVisitDateTime().empty())
                std::cout << ", Visit: " << patient->getVisitDateTime();
            std::cout << "\n";
            copy.pop();
            pos++;
        }
    }

    bool isSpecialist() const {
        std::string spec = specialization;
        std::transform(spec.begin(), spec.end(), spec.begin(), ::tolower);
        return spec != "ogólny" && spec != "ogolny" && spec != "lekarz rodzinny";
    }
};

#endif // DOCTOR_H