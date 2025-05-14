#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include "HospitalSystem.h"

int main() {
    HospitalSystem hospitalSystem;

    // Load data from files
    hospitalSystem.loadDoctors("doctors.txt");
    hospitalSystem.loadSymptoms("symptoms.txt");
    hospitalSystem.loadPatients("patients.txt");

    int choice;
    do {
        std::cout << "=== Medical Queue Management ===\n";
        std::cout << "1. Add a new patient\n";
        std::cout << "2. Assign patients to doctors\n";
        std::cout << "3. Schedule appointments\n";
        std::cout << "4. View all patients\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                hospitalSystem.addPatient();
                break;
            case 2:
                hospitalSystem.assignPatientsToDoctors();
                break;
            case 3:
                hospitalSystem.scheduleAppointments();
                break;
            case 4:
                hospitalSystem.printPatients();
                break;
            case 5:
                std::cout << "Exiting program.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}