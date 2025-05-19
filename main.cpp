#include "file_handler.h"
#include "hospital.h"
#include "patient.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <ctime>

// --- DODANE: obsługa biblioteki obliczeniowej ---
#include "parallel_calculator.h"
// ---

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
    std::cout << "8. Display matched patients for hospitals and doctors\n";
    std::cout << "9. Add patient to a doctor's queue\n";
    std::cout << "10. Show doctor's queue\n";
    std::cout << "11. Show doctor's schedule for a day\n";
    std::cout << "12. Wykonaj obliczenia naukowe (OpenMP)\n"; // Nowa opcja!
    std::cout << "13. Exit\n";
    std::cout << "Select an option: ";
}

// Pomocnicza funkcja do pobierania wskaźnika do szpitala po nazwie
Hospital* findHospitalByName(const std::vector<Hospital*>& hospitals, const std::string& hospitalName) {
    for (Hospital* h : hospitals) {
        if (h->getName() == hospitalName) {
            return h;
        }
    }
    return nullptr;
}

// Pomocnicza funkcja do pobierania wskaźnika do lekarza po nazwisku w szpitalu
Doctor* findDoctorByLastName(const std::vector<Doctor*>& doctors, const std::string& lastName) {
    for (Doctor* d : doctors) {
        if (d->getLastName() == lastName) {
            return d;
        }
    }
    return nullptr;
}

int main() {
    std::vector<Hospital*> hospitals; // Lista szpitali
    std::vector<Patient*> patients;   // Lista pacjentów
    std::unordered_map<std::string, std::vector<std::string>> symptomsMap; // Mapa symptomów

    int choice;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            std::string filePath;
            std::cout << "Enter the path to doctors.txt: ";
            std::cin >> filePath;
            FileHandler::loadDoctors(filePath, hospitals);
            std::cout << "Doctors loaded successfully.\n";
            break;
        }
        case 2: {
            std::string filePath;
            std::cout << "Enter the path to symptoms.txt: ";
            std::cin >> filePath;
            FileHandler::loadSymptoms(filePath, symptomsMap);
            std::cout << "Symptoms loaded successfully.\n";
            break;
        }
        case 3: {
            std::string filePath;
            std::cout << "Enter the path to patients.txt: ";
            std::cin >> filePath;
            FileHandler::loadPatients(filePath, patients);
            std::cout << "Patients loaded successfully.\n";
            break;
        }
        case 4: {
            for (const Hospital* hospital : hospitals) {
                std::cout << "Doctors in " << hospital->getName()
                          << " (" << hospital->getRegion() << "):" << std::endl;
                for (const Doctor* doctor : hospital->getDoctors()) {
                    std::cout << "Doctor: " << doctor->getFirstName() << " "
                              << doctor->getLastName() << ", Specialization: "
                              << doctor->getSpecialization() << std::endl;
                }
                std::cout << std::endl;
            }
            break;
        }
        case 5: {
            for (const Patient* patient : patients) {
                patient->printDetails();
            }
            break;
        }
        case 6: {
            for (Hospital* hospital : hospitals) {
                hospital->assignPatientsToDoctors();
            }
            break;
        }
        case 7: {
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
            std::cin.ignore();
            std::cout << "Enter patient's problem: ";
            std::getline(std::cin, problem);

            Patient* newPatient = new Patient(firstName, lastName, age, region, problem);
            patients.push_back(newPatient);
            std::cout << "Patient added successfully.\n";
            break;
        }
        case 8: {
            for (const Hospital* hospital : hospitals) {
                std::cout << "Hospital: " << hospital->getName() << " (" << hospital->getRegion() << ")\n";
                const auto& doctors = hospital->getDoctors();
                if (doctors.empty()) {
                    std::cout << "  No doctors in this hospital.\n";
                    continue;
                }
                for (const Doctor* doctor : doctors) {
                    doctor->printPatients();
                }
                std::cout << std::endl;
            }
            break;
        }
        case 9: {
            std::string hospitalName, doctorLastName;
            std::cout << "Enter hospital name: ";
            std::getline(std::cin, hospitalName);
            std::cout << "Enter doctor's last name: ";
            std::getline(std::cin, doctorLastName);

            Hospital* foundHospital = findHospitalByName(hospitals, hospitalName);
            if (!foundHospital) {
                std::cout << "Hospital not found.\n";
                break;
            }
            Doctor* foundDoctor = findDoctorByLastName(foundHospital->getDoctors(), doctorLastName);
            if (!foundDoctor) {
                std::cout << "Doctor not found.\n";
                break;
            }
            std::cout << "Select patient:\n";
            for (size_t i = 0; i < patients.size(); ++i) {
                std::cout << i+1 << ". ";
                patients[i]->printDetails();
            }
            int pIdx;
            std::cin >> pIdx;
            std::cin.ignore();
            if (pIdx < 1 || pIdx > (int)patients.size()) {
                std::cout << "Invalid patient index.\n";
                break;
            }
            foundDoctor->addToQueue(patients[pIdx-1]);
            std::cout << "Patient added to the doctor's queue.\n";
            break;
        }
        case 10: {
            std::string hospitalName, doctorLastName;
            std::cout << "Enter hospital name: ";
            std::getline(std::cin, hospitalName);
            std::cout << "Enter doctor's last name: ";
            std::getline(std::cin, doctorLastName);
            Hospital* foundHospital = findHospitalByName(hospitals, hospitalName);
            if (!foundHospital) {
                std::cout << "Hospital not found.\n";
                break;
            }
            Doctor* foundDoctor = findDoctorByLastName(foundHospital->getDoctors(), doctorLastName);
            if (!foundDoctor) {
                std::cout << "Doctor not found.\n";
                break;
            }
            foundDoctor->printQueue();
            break;
        }
        case 11: {
            std::string hospitalName, doctorLastName, date;
            std::cout << "Enter hospital name: ";
            std::getline(std::cin, hospitalName);
            std::cout << "Enter doctor's last name: ";
            std::getline(std::cin, doctorLastName);
            std::cout << "Enter date (YYYY-MM-DD): ";
            std::getline(std::cin, date);

            int year, month, day;
            char dash1, dash2;
            std::istringstream iss(date);
            if (!(iss >> year >> dash1 >> month >> dash2 >> day) || dash1 != '-' || dash2 != '-') {
                std::cout << "Invalid date format.\n";
                break;
            }

            std::tm tm = {};
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            tm.tm_hour = 12;
            mktime(&tm);

            if (tm.tm_wday == 0 || tm.tm_wday == 6) {
                std::cout << "Doctors do not work on weekends.\n";
                break;
            }

            Hospital* foundHospital = findHospitalByName(hospitals, hospitalName);
            if (!foundHospital) {
                std::cout << "Hospital not found.\n";
                break;
            }
            Doctor* foundDoctor = findDoctorByLastName(foundHospital->getDoctors(), doctorLastName);
            if (!foundDoctor) {
                std::cout << "Doctor not found.\n";
                break;
            }
            foundDoctor->printSchedule(date);
            break;
        }
        case 12: {
            // --- NOWA OPCJA: obliczenia naukowe OpenMP ---
            size_t n;
            int percent;
            std::cout << "Ile danych losowych wygenerować? ";
            std::cin >> n;
            std::cout << "Jaki procent CPU wykorzystać (1-100)? ";
            std::cin >> percent;
            init_data(n);
            set_cpu_usage_percent(percent);
            double czas = run_computation();
            std::cout << "Obliczenia trwały: " << czas << " sekundy\n";
            break;
        }
        case 13: {
            std::cout << "Exiting program. Goodbye!\n";
            break;
        }
        default:
            std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 13);

    for (Hospital* hospital : hospitals) {
        delete hospital;
    }
    for (Patient* patient : patients) {
        delete patient;
    }

    return 0;
}