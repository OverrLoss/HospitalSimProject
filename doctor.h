#ifndef DOCTOR_H
#define DOCTOR_H

#include "patient.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <ctime>

// Klasa reprezentująca lekarza
class Doctor {
private:
    std::string firstName;
    std::string lastName;
    std::string specialization;
    std::vector<Patient*> patients; // Lista pacjentów przypisanych do lekarza
    std::queue<Patient*> patientQueue; // Kolejka pacjentów do lekarza

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
        patientQueue.push(patient);
    }

    // Dodanie pacjenta tylko do kolejki (np. zapisanie się na wizytę)
    void addToQueue(Patient* patient) {
        patientQueue.push(patient);
    }

    // Wyświetlenie wszystkich pacjentów lekarza
    void printPatients() const {
        std::cout << "Patients of Dr. " << firstName << " " << lastName << " (" << specialization << "):\n";
        for (const Patient* patient : patients) {
            std::cout << " - " << patient->getFirstName() << " " << patient->getLastName() << ", Age: " << patient->getAge()
                      << ", Problem: " << patient->getProblem() << "\n";
        }
    }

    // Wyświetlenie kolejki pacjentów
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
                      << ", Age: " << patient->getAge() << ", Problem: " << patient->getProblem() << "\n";
            copy.pop();
            pos++;
        }
    }

    // Wyświetlenie harmonogramu wizyt na dany dzień (z uwzględnieniem typu lekarza i 30-minutowych wizyt)
    void printSchedule(const std::string& date) const {
        std::cout << "Schedule for Dr. " << firstName << " " << lastName << " (" << specialization << ") on " << date << ":\n";
        // Określenie godzin pracy na podstawie specjalizacji
        int startHour = isSpecialist() ? 8 : 10;
        int endHour = isSpecialist() ? 16 : 18;
        int totalSlots = (endHour - startHour) * 2; // 30-minutowe sloty
        std::queue<Patient*> copy = patientQueue;

        char buf[6];
        for (int i = 0; i < totalSlots; ++i) {
            int hour = startHour + (i / 2);
            int minute = (i % 2) * 30;
            snprintf(buf, sizeof(buf), "%02d:%02d", hour, minute);
            std::cout << buf << " - ";
            if (!copy.empty()) {
                const Patient* patient = copy.front();
                std::cout << patient->getFirstName() << " " << patient->getLastName() << " (" << patient->getProblem() << ")\n";
                copy.pop();
            } else {
                std::cout << "free\n";
            }
        }
    }

    // Sprawdzenie czy lekarz jest specjalistą
    bool isSpecialist() const {
        // Zakładamy, że specjalizacje inne niż "Ogólny" to specjaliści
        std::string spec = specialization;
        for (char& c : spec) c = std::tolower(c);
        return spec != "ogólny" && spec != "ogolny" && spec != "lekarz rodzinny";
    }
};

#endif // DOCTOR_H