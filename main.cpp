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
#include <cctype>
#include <ctime>
#include <iomanip>
#include <fstream>

// --- List of Polish voivodeships (województwa) ---
const std::vector<std::string> wojewodztwa = {
    "Dolnoslaskie", "Kujawsko-pomorskie", "Lubelskie", "Lubuskie", "Lodzkie",
    "Malopolskie", "Mazowieckie", "Opolskie", "Podkarpackie", "Podlaskie",
    "Pomorskie", "Slaskie", "Swietokrzyskie", "Warminsko-mazurskie",
    "Wielkopolskie", "Zachodniopomorskie"
};

enum class Language { EN, PL };

int selectWojewodztwo(Language lang) {
    std::cout << (lang == Language::EN ? "Select region:" : "Wybierz województwo:") << "\n";
    for (int i = 0; i < wojewodztwa.size(); ++i) {
        std::cout << i+1 << ". " << wojewodztwa[i] << "\n";
    }
    int w;
    std::cout << (lang == Language::EN ? "Enter number: " : "Podaj numer: ");
    std::cin >> w; std::cin.ignore();
    if (w < 1 || w > (int)wojewodztwa.size()) return -1;
    return w-1;
}

int selectHospital(const std::vector<Hospital*>& hospitals, const std::string& region, Language lang) {
    std::vector<Hospital*> filtered;
    for (auto* h : hospitals) {
        if (h->getRegion() == region) filtered.push_back(h);
    }
    if (filtered.empty()) {
        std::cout << (lang == Language::EN ? "No hospitals in this region.\n" : "Brak szpitali w tym województwie.\n");
        return -1;
    }
    std::cout << (lang == Language::EN ? "Select hospital:" : "Wybierz szpital:") << "\n";
    for (int i = 0; i < filtered.size(); ++i) {
        std::cout << i+1 << ". " << filtered[i]->getName() << "\n";
    }
    int s;
    std::cout << (lang == Language::EN ? "Enter number: " : "Podaj numer: ");
    std::cin >> s; std::cin.ignore();
    if (s < 1 || s > (int)filtered.size()) return -1;
    for (int i = 0, j = 0; i < hospitals.size(); ++i) {
        if (hospitals[i]->getRegion() == region) {
            if (j == s-1) return i;
            ++j;
        }
    }
    return -1;
}

int selectDoctor(const std::vector<Doctor*>& doctors, Language lang) {
    if (doctors.empty()) {
        std::cout << (lang == Language::EN ? "No doctors available.\n" : "Brak lekarzy.\n");
        return -1;
    }
    std::cout << (lang == Language::EN ? "Select doctor:" : "Wybierz lekarza:") << "\n";
    for (int i = 0; i < doctors.size(); ++i) {
        std::cout << i+1 << ". " << doctors[i]->getFirstName()
                  << " " << doctors[i]->getLastName()
                  << " (" << doctors[i]->getSpecialization() << ")\n";
    }
    int d;
    std::cout << (lang == Language::EN ? "Enter number: " : "Podaj numer: ");
    std::cin >> d; std::cin.ignore();
    if (d < 1 || d > (int)doctors.size()) return -1;
    return d-1;
}

static std::string to_lower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}

std::string getSpecializationForProblem(
    const std::string& problem,
    const std::unordered_map<std::string, std::vector<std::string>>& symptomsMap
) {
    std::string probL = to_lower(problem);
    for (const auto& [spec, symptoms] : symptomsMap) {
        for (const std::string& sym : symptoms) {
            std::string symL = to_lower(sym);
            if (probL.find(symL) != std::string::npos || symL.find(probL) != std::string::npos)
                return spec;
        }
    }
    return "Lekarz ogólny";
}

void assignPatientsBySymptomAndLoad(
    std::vector<Hospital*>& hospitals,
    std::vector<Patient*>& patients,
    const std::unordered_map<std::string, std::vector<std::string>>& symptomsMap
) {
    for (Patient* patient : patients) {
        std::vector<Hospital*> regionHospitals;
        for (Hospital* h : hospitals) {
            if (to_lower(h->getRegion()) == to_lower(patient->getRegion()))
                regionHospitals.push_back(h);
        }
        if (regionHospitals.empty()) continue;

        std::string specialization = getSpecializationForProblem(patient->getProblem(), symptomsMap);

        std::vector<Doctor*> doctorsSpecialist;
        for (Hospital* h : regionHospitals) {
            for (Doctor* d : h->getDoctors()) {
                if (to_lower(d->getSpecialization()) == to_lower(specialization)) {
                    doctorsSpecialist.push_back(d);
                }
            }
        }
        Doctor* selectedDoctor = nullptr;
        if (!doctorsSpecialist.empty()) {
            selectedDoctor = *std::min_element(doctorsSpecialist.begin(), doctorsSpecialist.end(),
                [](Doctor* a, Doctor* b) {
                    return a->getPatientsCount() < b->getPatientsCount();
                });
        } else {
            std::vector<Doctor*> doctorsGeneral;
            for (Hospital* h : regionHospitals) {
                for (Doctor* d : h->getDoctors()) {
                    std::string spec = to_lower(d->getSpecialization());
                    if (spec == "lekarz ogólny" || spec == "lekarz ogolny" || spec == "lekarz rodzinny")
                        doctorsGeneral.push_back(d);
                }
            }
            if (!doctorsGeneral.empty()) {
                selectedDoctor = *std::min_element(doctorsGeneral.begin(), doctorsGeneral.end(),
                    [](Doctor* a, Doctor* b) {
                        return a->getPatientsCount() < b->getPatientsCount();
                    });
            }
        }
        if (selectedDoctor)
            selectedDoctor->addPatient(patient);
    }
}

void assignVisitDates(const std::vector<Hospital*>& hospitals) {
    std::time_t now = std::time(nullptr);
    std::tm tm_date = *std::localtime(&now);
    tm_date.tm_mday += 1;
    std::mktime(&tm_date);

    for (auto* hospital : hospitals) {
        for (auto* doctor : hospital->getDoctors()) {
            int startHour = doctor->isSpecialist() ? 8 : 10;
            int endHour = doctor->isSpecialist() ? 16 : 20;
            int slotsPerDay = (endHour - startHour) * 2;

            int slot = 0;
            int dayOffset = 0;
            for (auto* patient : doctor->getPatients()) {
                int slotInDay = slot % slotsPerDay;
                dayOffset = slot / slotsPerDay;
                int hour = startHour + (slotInDay / 2);
                int minute = (slotInDay % 2) * 30;

                std::tm tm_visit = tm_date;
                tm_visit.tm_mday += dayOffset;
                std::mktime(&tm_visit);
                char datebuf[20];
                std::strftime(datebuf, sizeof(datebuf), "%Y-%m-%d", &tm_visit);
                std::ostringstream oss;
                oss << datebuf << " ";
                oss << std::setw(2) << std::setfill('0') << hour << ":"
                    << std::setw(2) << std::setfill('0') << minute;
                patient->setVisitDateTime(oss.str());
                ++slot;
            }
        }
    }
}

void exportQueuesToTxt(const std::vector<Hospital*>& hospitals, Language lang) {
    std::string filename;
    if (lang == Language::EN)
        std::cout << "Enter filename for export (default: queues.txt): ";
    else
        std::cout << "Podaj nazwę pliku do eksportu (domyślnie: kolejki.txt): ";
    std::getline(std::cin, filename);
    if (filename.empty())
        filename = (lang == Language::EN) ? "queues.txt" : "kolejki.txt";
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cout << (lang == Language::EN ? "Cannot open file!\n" : "Nie można otworzyć pliku!\n");
        return;
    }

    for(const auto* h : hospitals) {
        bool anyPatient = false;
        for (const auto* d : h->getDoctors())
            if (d->getPatientsCount() > 0) anyPatient = true;
        if (!anyPatient) {
            ofs << (lang == Language::EN ? "No patients in hospital: " : "Brak pacjentów w szpitalu: ")
                << h->getName() << "\n";
            continue;
        }
        ofs << (lang == Language::EN ? "Hospital: " : "Szpital: ")
            << h->getName() << " (" << h->getRegion() << ")\n";
        for(const auto* d : h->getDoctors()) {
            if (d->getPatientsCount() == 0) continue;
            ofs << "Queue for Dr. " << d->getFirstName() << " " << d->getLastName()
                << " (" << d->getSpecialization() << "):\n";
            int pos = 1;
            for (const auto* p : d->getPatients()) {
                ofs << " " << pos++ << ". " << p->getFirstName() << " " << p->getLastName()
                    << ", Age: " << p->getAge()
                    << ", Problem: " << p->getProblem()
                    << ", Visit: " << p->getVisitDateTime() << "\n";
            }
        }
    }
    ofs.close();
    std::cout << (lang == Language::EN ? "Exported to file: " : "Wyeksportowano do pliku: ") << filename << "\n";
}

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
            "16. Export queues to TXT file\n"
            "17. Run scientific computation\n"
            "18. Exit program\n"
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
            "16. Eksportuj kolejki do pliku TXT\n"
            "17. Wykonaj obliczenie naukowe\n"
            "18. Zakoncz program\n"
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
                int widx = selectWojewodztwo(lang);
                if (widx == -1) break;
                std::string name;
                std::cout << (lang == Language::EN ? "Hospital name: " : "Nazwa szpitala: ");
                std::getline(std::cin, name);
                hospitals.push_back(new Hospital(name, wojewodztwa[widx]));
                break;
            }
            case 3: {
                int widx = selectWojewodztwo(lang);
                if (widx == -1) break;
                int hidx = selectHospital(hospitals, wojewodztwa[widx], lang);
                if (hidx == -1) break;
                Hospital* hospital = hospitals[hidx];
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
                std::cin.ignore();
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
                int widx = selectWojewodztwo(lang);
                if (widx == -1) break;
                std::string firstName, lastName, region = wojewodztwa[widx], problem;
                int age;
                if (lang == Language::EN) {
                    std::cout << "Patient first name: "; std::cin >> firstName;
                    std::cout << "Patient last name: "; std::cin >> lastName;
                    std::cout << "Age: "; std::cin >> age;
                    std::cout << "Problem: "; std::cin.ignore(); std::getline(std::cin, problem);
                } else {
                    std::cout << "Imie pacjenta: "; std::cin >> firstName;
                    std::cout << "Nazwisko pacjenta: "; std::cin >> lastName;
                    std::cout << "Wiek: "; std::cin >> age;
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
                    h->printDoctors();
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
                assignPatientsBySymptomAndLoad(hospitals, patients, symptomsMap);
                // Nie wyświetlaj danych (dane wyświetla case 12)
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
                assignVisitDates(hospitals);
                // Nie wyświetlaj danych (dane wyświetla case 14)
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
                    bool anyPatient = false;
                    for (const auto* d : h->getDoctors())
                        if (d->getPatientsCount() > 0) anyPatient = true;
                    if (!anyPatient) {
                        std::cout << (lang == Language::EN ? "No patients in hospital: " : "Brak pacjentów w szpitalu: ")
                                  << h->getName() << "\n";
                        continue;
                    }
                    std::cout << (lang == Language::EN ? "Hospital: " : "Szpital: ")
                              << h->getName() << " (" << h->getRegion() << ")\n";
                    for(const auto* d : h->getDoctors()) d->printQueue();
                }
                break;
            }
            case 15: {
                int widx = selectWojewodztwo(lang);
                if (widx == -1) break;
                int hidx = selectHospital(hospitals, wojewodztwa[widx], lang);
                if (hidx == -1) break;
                Hospital* hospital = hospitals[hidx];
                int didx = selectDoctor(hospital->getDoctors(), lang);
                if (didx == -1) break;
                hospital->getDoctors()[didx]->printQueue();
                break;
            }
            case 16: {
                exportQueuesToTxt(hospitals, lang);
                break;
            }
            case 17: {
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
            case 18: {
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
    } while(choice != 18);

    for(auto* h : hospitals) delete h;
    for(auto* p : patients) delete p;
    return 0;
}