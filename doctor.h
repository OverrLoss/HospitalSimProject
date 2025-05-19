#ifndef DOCTOR_H
#define DOCTOR_H

#include "person.h"
#include "patient.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

class Doctor : public Person {
protected:
    std::string specialization;
    std::vector<Patient*> patients; // Lista pacjentów przypisanych do lekarza
    std::queue<Patient*> patientQueue; // Kolejka pacjentów do lekarza

public:
    Doctor() : Person(), specialization("") {}
    Doctor(const std::string& firstName, const std::string& lastName, const std::string& specialization)
        : Person(firstName, lastName), specialization(specialization) {}
    Doctor(const Doctor& other)
        : Person(other), specialization(other.specialization), patients(other.patients) {}
    virtual ~Doctor() {
        // Przykład zwalniania dynamicznej pamięci (jeśli Doctor alokował pacjentów)
        // for (Patient* p : patients) delete p;
    }

    const std::string& getSpecialization() const { return specialization; }

    virtual void addPatient(Patient* patient) {
        patients.push_back(patient);
        patientQueue.push(patient);
    }

    int getPatientsCount() const { return patients.size(); }
    const std::vector<Patient*>& getPatients() const { return patients; }

    void printPatients() const {
        std::cout << "Patients of Dr. " << firstName << " " << lastName << " (" << specialization << "):\n";
        for (const Patient* patient : patients) {
            patient->printDetails();
        }
    }

    virtual void printDetails() const override {
        std::cout << "Doctor: " << firstName << " " << lastName << ", Specialization: " << specialization << std::endl;
    }

    virtual void printQueue() const {
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

// Przykład klasy pochodnej
class Surgeon : public Doctor {
private:
    std::string surgeryType;

public:
    Surgeon() : Doctor(), surgeryType("general") {}
    Surgeon(const std::string& firstName, const std::string& lastName, const std::string& specialization, const std::string& surgeryType)
        : Doctor(firstName, lastName, specialization), surgeryType(surgeryType) {}
    Surgeon(const Surgeon& other)
        : Doctor(other), surgeryType(other.surgeryType) {}
    ~Surgeon() override {}

    void printDetails() const override {
        std::cout << "Surgeon: " << firstName << " " << lastName << ", Spec: " << specialization
                  << ", Surgery type: " << surgeryType << std::endl;
    }

    void performSurgery() const {
        std::cout << "Performing surgery type: " << surgeryType << std::endl;
    }
};

#endif // DOCTOR_H