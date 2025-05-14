#include "file_handler.h"
#include "hospital.h"
#include "patient.h"
#include <iostream>
#include <vector>
#include <unordered_map>

// Funkcja pomocnicza do wyświetlania menu
void displayMenu() {
    std::cout << "\n--- Hospital Queue Management System ---\n";
    std::cout << "1. Load doctors from file\n";
    std::cout << "2. Load symptoms from file\n";
    std::cout << "3. Load patients from file\n";
    std::cout << "4. Display hospitals and doctors\n";
    std::cout << "5. Display patients\n";
    std::cout << "6. Assign patients to doctors\n";
    std::cout << "7. Add a new patient\n";
    std::cout << "8. Exit\n";
    std::cout << "Select an option: ";
}

int main() {
    std::vector<Hospital*> hospitals; // Lista szpitali
    std::vector<Patient*> patients;   // Lista pacjentów
    std::unordered_map<std::string, std::vector<std::string>> symptomsMap; // Mapa symptomów

    int choice;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Ignorowanie znaku nowej linii po wyborze

        switch (choice) {
        case 1: {
            // Wczytywanie lekarzy
            std::string filePath;
            std::cout << "Enter the path to doctors.txt: ";
            std::cin >> filePath;
            FileHandler::loadDoctors(filePath, hospitals);
            std::cout << "Doctors loaded successfully.\n";
            break;
        }
        case 2: {
            // Wczytywanie symptomów
            std::string filePath;
            std::cout << "Enter the path to symptoms.txt: ";
            std::cin >> filePath;
            FileHandler::loadSymptoms(filePath, symptomsMap);
            std::cout << "Symptoms loaded successfully.\n";
            break;
        }
        case 3: {
            // Wczytywanie pacjentów
            std::string filePath;
            std::cout << "Enter the path to patients.txt: ";
            std::cin >> filePath;
            FileHandler::loadPatients(filePath, patients);
            std::cout << "Patients loaded successfully.\n";
            break;
        }
        case 4: {
            // Wyświetlanie szpitali i lekarzy
            for (const Hospital* hospital : hospitals) {
                hospital->printDoctors();
            }
            break;
        }
        case 5: {
            // Wyświetlanie pacjentów
            for (const Patient* patient : patients) {
                patient->printDetails();
            }
            break;
        }
        case 6: {
            // Przypisywanie pacjentów do lekarzy
            for (Hospital* hospital : hospitals) {
                hospital->assignPatientsToDoctors();
            }
            break;
        }
        case 7: {
            // Dodawanie nowego pacjenta
            std::string firstName, lastName, region, problem;
            int age;
            std::cout << "Enter patient's first name: ";
            std::cin >> firstName;
            std::cout << "Enter patient's last name: ";
            std::cin >> lastName;
            std::cout << "Enter patient's age: ";
            std::cin >> age;
            std::cout << "Enter patient's region: ";
            std::cin >> region;
            std::cin.ignore(); // Ignorowanie znaku nowej linii
            std::cout << "Enter patient's problem: ";
            std::getline(std::cin, problem);

            Patient* newPatient = new Patient(firstName, lastName, age, region, problem);
            patients.push_back(newPatient);
            std::cout << "Patient added successfully.\n";
            break;
        }
        case 8: {
            std::cout << "Exiting program. Goodbye!\n";
            break;
        }
        default:
            std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 8);

    // Zwolnienie pamięci
    for (Hospital* hospital : hospitals) {
        delete hospital;
    }
    for (Patient* patient : patients) {
        delete patient;
    }

    return 0;
}