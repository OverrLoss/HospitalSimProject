#include "file_handler.h"
#include "hospital.h"
#include "doctor.h"
#include "patient.h"
#include "parallel_calculator.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <locale>
#include <algorithm>

enum class Language { EN, PL };

void displayMenu(Language lang) {
    if (lang == Language::EN) {
        std::cout <<
            "\n--- Hospital Management Menu ---\n"
            "1. Load doctors from file\n"
            "2. Add hospital\n"
            "3. Add doctor\n"
            "4. Load patients from file\n"
            "5. Add patient to list\n"
            "6. Load symptoms from file\n"
            "7. Add symptom\n"
            "8. Show hospitals and assigned doctors (sorted)\n"
            "9. Show patients\n"
            "10. Show symptoms\n"
            "11. Assign patients to doctors\n"
            "12. Show patients assigned to doctors (sorted)\n"
            "13. Create queues\n"
            "14. Show queue for all doctors (sorted)\n"
            "15. Show queue for selected doctor\n"
            "16. Run scientific computation\n"
            "17. Exit program\n"
            "Select option: ";
    } else {
        std::cout <<
            "\n--- Menu zarzadzania szpitalem ---\n"
            "1. Wczytaj lekarzy z pliku\n"
            "2. Dodaj szpital\n"
            "3. Dodaj lekarza\n"
            "4. Wczytaj pacjentow z pliku\n"
            "5. Dodaj pacjenta do listy\n"
            "6. Wczytaj objawy z pliku\n"
            "7. Dodaj objaw\n"
            "8. Wyswietl szpitale i przypisanych lekarzy (posortowane)\n"
            "9. Wyswietl pacjentow\n"
            "10. Wyswietl objawy\n"
            "11. Przypisz pacjentow do lekarzy\n"
            "12. Wyswietl pacjentow przypisanych do lekarzy (posortowane)\n"
            "13. Utworz kolejki\n"
            "14. Wyswietl kolejki wszystkich lekarzy (posortowane)\n"
            "15. Wyswietl kolejke lekarza\n"
            "16. Wykonaj obliczenie naukowe\n"
            "17. Zakoncz program\n"
            "Wybierz opcje: ";
    }
}

int main() {
    std::locale::global(std::locale(""));
    std::vector<Hospital*> hospitals;
    std::vector<Patient*> patients;
    std::unordered_map<std::string, std::vector<std::string>> symptomsMap;

    Language lang = Language::EN;
    std::cout << "Select language / Wybierz jezyk:\n1. English\n2. Polski\nChoice: ";
    int langChoice;
    std::cin >> langChoice;
    std::cin.ignore();
    if (langChoice == 2) lang = Language::PL;

    int choice = 0;
    do {
        displayMenu(lang);
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: {
                std::string filePath;
                if (lang == Language::EN) {
                    std::cout << "Load doctors from:\n";
                    std::cout << "1. Default file (doctors.txt)\n";
                    std::cout << "2. Custom file\n";
                    std::cout << "Select option: ";
                } else {
                    std::cout << "Wczytaj lekarzy z:\n";
                    std::cout << "1. Domyslny plik (doctors.txt)\n";
                    std::cout << "2. Wlasny plik\n";
                    std::cout << "Wybierz opcje: ";
                }
                int fileChoice;
                std::cin >> fileChoice;
                std::cin.ignore();
                if (fileChoice == 1) {
                    filePath = "doctors.txt";
                } else {
                    if (lang == Language::EN)
                        std::cout << "Enter path to doctors file: ";
                    else
                        std::cout << "Podaj sciezke do pliku z lekarzami: ";
                    std::getline(std::cin, filePath);
                }
                FileHandler::loadDoctors(filePath, hospitals);
                break;
            }
            case 2: {
                std::string region, name;
                if (lang == Language::EN) {
                    std::cout << "Region: "; std::cin >> region;
                    std::cout << "Hospital name: "; std::cin.ignore(); std::getline(std::cin, name);
                } else {
                    std::cout << "Wojewodztwo: "; std::cin >> region;
                    std::cout << "Nazwa szpitala: "; std::cin.ignore(); std::getline(std::cin, name);
                }
                hospitals.push_back(new Hospital(name, region));
                break;
            }
            case 3: {
                std::string region, hospitalName;
                if (lang == Language::EN) {
                    std::cout << "Region: "; std::cin >> region;
                    std::cout << "Hospital name: "; std::cin.ignore(); std::getline(std::cin, hospitalName);
                } else {
                    std::cout << "Wojewodztwo: "; std::cin >> region;
                    std::cout << "Nazwa szpitala: "; std::cin.ignore(); std::getline(std::cin, hospitalName);
                }
                Hospital* hospital = nullptr;
                for(auto* h : hospitals)
                    if(h->getRegion() == region && h->getName() == hospitalName) hospital = h;
                if(!hospital) {
                    if (lang == Language::EN)
                        std::cout << "Hospital not found.\n";
                    else
                        std::cout << "Nie znaleziono szpitala.\n";
                    break;
                }
                std::string firstName, lastName, specialization;
                if (lang == Language::EN) {
                    std::cout << "Doctor first name: "; std::cin >> firstName;
                    std::cout << "Doctor last name: "; std::cin >> lastName;
                    std::cout << "Specialization: "; std::cin >> specialization;
                } else {
                    std::cout << "Imie lekarza: "; std::cin >> firstName;
                    std::cout << "Nazwisko lekarza: "; std::cin >> lastName;
                    std::cout << "Specjalizacja: "; std::cin >> specialization;
                }
                hospital->addDoctor(new Doctor(firstName, lastName, specialization));
                break;
            }
            case 4: {
                std::string filePath;
                if (lang == Language::EN) {
                    std::cout << "Load patients from:\n";
                    std::cout << "1. Default file (patients.txt)\n";
                    std::cout << "2. Custom file\n";
                    std::cout << "Select option: ";
                } else {
                    std::cout << "Wczytaj pacjentow z:\n";
                    std::cout << "1. Domyslny plik (patients.txt)\n";
                    std::cout << "2. Wlasny plik\n";
                    std::cout << "Wybierz opcje: ";
                }
                int fileChoice;
                std::cin >> fileChoice;
                std::cin.ignore();
                if (fileChoice == 1) {
                    filePath = "patients.txt";
                } else {
                    if (lang == Language::EN)
                        std::cout << "Enter path to patients file: ";
                    else
                        std::cout << "Podaj sciezke do pliku z pacjentami: ";
                    std::getline(std::cin, filePath);
                }
                FileHandler::loadPatients(filePath, patients);
                break;
            }
            case 5: {
                std::string firstName, lastName, region, problem;
                int age;
                if (lang == Language::EN) {
                    std::cout << "Patient first name: "; std::cin >> firstName;
                    std::cout << "Patient last name: "; std::cin >> lastName;
                    std::cout << "Age: "; std::cin >> age;
                    std::cout << "Region: "; std::cin >> region;
                    std::cout << "Problem: "; std::cin.ignore(); std::getline(std::cin, problem);
                } else {
                    std::cout << "Imie pacjenta: "; std::cin >> firstName;
                    std::cout << "Nazwisko pacjenta: "; std::cin >> lastName;
                    std::cout << "Wiek: "; std::cin >> age;
                    std::cout << "Wojewodztwo: "; std::cin >> region;
                    std::cout << "Problem: "; std::cin.ignore(); std::getline(std::cin, problem);
                }
                patients.push_back(new Patient(firstName, lastName, age, region, problem));
                break;
            }
            case 6: {
                std::string filePath;
                if (lang == Language::EN) {
                    std::cout << "Load symptoms from:\n";
                    std::cout << "1. Default file (symptoms.txt)\n";
                    std::cout << "2. Custom file\n";
                    std::cout << "Select option: ";
                } else {
                    std::cout << "Wczytaj objawy z:\n";
                    std::cout << "1. Domyslny plik (symptoms.txt)\n";
                    std::cout << "2. Wlasny plik\n";
                    std::cout << "Wybierz opcje: ";
                }
                int fileChoice;
                std::cin >> fileChoice;
                std::cin.ignore();
                if (fileChoice == 1) {
                    filePath = "symptoms.txt";
                } else {
                    if (lang == Language::EN)
                        std::cout << "Enter path to symptoms file: ";
                    else
                        std::cout << "Podaj sciezke do pliku z objawami: ";
                    std::getline(std::cin, filePath);
                }
                FileHandler::loadSymptoms(filePath, symptomsMap);
                break;
            }
            case 7: {
                std::string specialist, symptom;
                if (lang == Language::EN) {
                    std::cout << "Specialist: "; std::cin >> specialist;
                    std::cout << "Symptom: "; std::cin >> symptom;
                } else {
                    std::cout << "Specjalista: "; std::cin >> specialist;
                    std::cout << "Objaw: "; std::cin >> symptom;
                }
                symptomsMap[specialist].push_back(symptom);
                break;
            }
            case 8: {
                std::vector<Hospital*> sortedHospitals = hospitals;
                std::sort(sortedHospitals.begin(), sortedHospitals.end(), [](const Hospital* a, const Hospital* b) {
                    if (a->getRegion() == b->getRegion())
                        return a->getName() < b->getName();
                    return a->getRegion() < b->getRegion();
                });

                for(const auto* h : sortedHospitals) {
                    h->printDoctors(); // Nagłówek i lekarze, bez powtarzania osobnej linii z nazwą szpitala
                }
                break;
            }
            case 9: {
                for(const auto* p : patients) p->printDetails();
                break;
            }
            case 10: {
                for(const auto& [spec, symptoms] : symptomsMap) {
                    std::cout << spec << ": ";
                    for(const auto& s : symptoms) std::cout << s << ", ";
                    std::cout << "\n";
                }
                break;
            }
            case 11: {
                for(auto* h : hospitals) {
                    for(auto* p : patients) {
                        if(p->getRegion() == h->getRegion()) {
                            auto docs = h->getDoctors();
                            if(!docs.empty()) docs[0]->addPatient(p);
                        }
                    }
                }
                break;
            }
            case 12: {
                std::vector<Hospital*> sortedHospitals = hospitals;
                std::sort(sortedHospitals.begin(), sortedHospitals.end(), [](const Hospital* a, const Hospital* b) {
                    if (a->getRegion() == b->getRegion())
                        return a->getName() < b->getName();
                    return a->getRegion() < b->getRegion();
                });

                for(const auto* h : sortedHospitals) {
                    std::cout << (lang == Language::EN ? "Hospital: " : "Szpital: ")
                              << h->getName() << " (" << h->getRegion() << ")\n";
                    for(const auto* d : h->getDoctors()) d->printPatients();
                }
                break;
            }
            case 13: {
                for(auto* h : hospitals)
                    for(auto* d : h->getDoctors())
                        d->printQueue();
                break;
            }
            case 14: {
                std::vector<Hospital*> sortedHospitals = hospitals;
                std::sort(sortedHospitals.begin(), sortedHospitals.end(), [](const Hospital* a, const Hospital* b) {
                    if (a->getRegion() == b->getRegion())
                        return a->getName() < b->getName();
                    return a->getRegion() < b->getRegion();
                });

                for(const auto* h : sortedHospitals) {
                    std::cout << (lang == Language::EN ? "Hospital: " : "Szpital: ")
                              << h->getName() << " (" << h->getRegion() << ")\n";
                    for(const auto* d : h->getDoctors()) d->printQueue();
                }
                break;
            }
            case 15: {
                std::string region, hospitalName, lastName;
                if (lang == Language::EN) {
                    std::cout << "Region: "; std::cin >> region;
                    std::cout << "Hospital: "; std::cin.ignore(); std::getline(std::cin, hospitalName);
                    std::cout << "Doctor last name: "; std::getline(std::cin, lastName);
                } else {
                    std::cout << "Wojewodztwo: "; std::cin >> region;
                    std::cout << "Szpital: "; std::cin.ignore(); std::getline(std::cin, hospitalName);
                    std::cout << "Nazwisko lekarza: "; std::getline(std::cin, lastName);
                }
                for(const auto* h : hospitals) {
                    if(h->getRegion() == region && h->getName() == hospitalName) {
                        for(const auto* d : h->getDoctors()) {
                            if(d->getLastName() == lastName) d->printQueue();
                        }
                    }
                }
                break;
            }
            case 16: {
                size_t n;
                int percent;
                if (lang == Language::EN) {
                    std::cout << "How many random data values to generate? ";
                    std::cin >> n;
                    std::cout << "What percent of CPU to use (1-100)? ";
                    std::cin >> percent;
                } else {
                    std::cout << "Ile losowych wartosci danych wygenerowac? ";
                    std::cin >> n;
                    std::cout << "Jaki procent CPU uzyc (1-100)? ";
                    std::cin >> percent;
                }
                init_data(n);
                set_cpu_usage_percent(percent);
                double t = run_computation();
                if (lang == Language::EN)
                    std::cout << "Computation took: " << t << " seconds\n";
                else
                    std::cout << "Obliczenia trwaly: " << t << " sekund\n";
                break;
            }
            case 17: {
                if (lang == Language::EN)
                    std::cout << "Program ended!\n";
                else
                    std::cout << "Program zakonczony!\n";
                break;
            }
            default:
                if (lang == Language::EN)
                    std::cout << "Invalid option!\n";
                else
                    std::cout << "Nieprawidlowa opcja!\n";
        }
    } while(choice != 17);

    for(auto* h : hospitals) delete h;
    for(auto* p : patients) delete p;
    return 0;
}