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
#include <numeric>
#include <map>

// --- Lista polskich województw ---
const std::vector<std::string> wojewodztwa = {
    "Dolnoslaskie", "Kujawsko-pomorskie", "Lubelskie", "Lubuskie", "Lodzkie",
    "Malopolskie", "Mazowieckie", "Opolskie", "Podkarpackie", "Podlaskie",
    "Pomorskie", "Slaskie", "Swietokrzyskie", "Warminsko-mazurskie",
    "Wielkopolskie", "Zachodniopomorskie"
};

int selectWojewodztwo() {
    std::cout << "Wybierz województwo:\n";
    for (int i = 0; i < wojewodztwa.size(); ++i) {
        std::cout << i+1 << ". " << wojewodztwa[i] << "\n";
    }
    int w;
    std::cout << "Podaj numer: ";
    std::cin >> w; std::cin.ignore();
    if (w < 1 || w > (int)wojewodztwa.size()) return -1;
    return w-1;
}

int selectHospital(const std::vector<Hospital*>& hospitals, const std::string& region) {
    std::vector<Hospital*> filtered;
    for (auto* h : hospitals) {
        if (h->getRegion() == region) filtered.push_back(h);
    }
    if (filtered.empty()) {
        std::cout << "Brak szpitali w tym województwie.\n";
        return -1;
    }
    std::cout << "Wybierz szpital:\n";
    for (int i = 0; i < filtered.size(); ++i) {
        std::cout << i+1 << ". " << filtered[i]->getName() << "\n";
    }
    int s;
    std::cout << "Podaj numer: ";
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

int selectDoctor(const std::vector<Doctor*>& doctors) {
    if (doctors.empty()) {
        std::cout << "Brak lekarzy.\n";
        return -1;
    }
    std::cout << "Wybierz lekarza:\n";
    for (int i = 0; i < doctors.size(); ++i) {
        std::cout << i+1 << ". " << doctors[i]->getFirstName()
                  << " " << doctors[i]->getLastName()
                  << " (" << doctors[i]->getSpecialization() << ")\n";
    }
    int d;
    std::cout << "Podaj numer: ";
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

void exportQueuesToTxt(const std::vector<Hospital*>& hospitals) {
    std::string filename;
    std::cout << "Podaj nazwę pliku do eksportu (domyślnie: kolejki.txt): ";
    std::getline(std::cin, filename);
    if (filename.empty())
        filename = "kolejki.txt";
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cout << "Nie można otworzyć pliku!\n";
        return;
    }

    for(const auto* h : hospitals) {
        bool anyPatient = false;
        for (const auto* d : h->getDoctors())
            if (d->getPatientsCount() > 0) anyPatient = true;
        if (!anyPatient) {
            ofs << "Brak pacjentów w szpitalu: "
                << h->getName() << "\n";
            continue;
        }
        ofs << "Szpital: "
            << h->getName() << " (" << h->getRegion() << ")\n";
        for(const auto* d : h->getDoctors()) {
            if (d->getPatientsCount() == 0) continue;
            ofs << "Kolejka do dr " << d->getFirstName() << " " << d->getLastName()
                << " (" << d->getSpecialization() << "):\n";
            int pos = 1;
            for (const auto* p : d->getPatients()) {
                ofs << " " << pos++ << ". " << p->getFirstName() << " " << p->getLastName()
                    << ", Wiek: " << p->getAge()
                    << ", Problem: " << p->getProblem()
                    << ", Wizyta: " << p->getVisitDateTime() << "\n";
            }
        }
    }
    ofs.close();
    std::cout << "Wyeksportowano do pliku: " << filename << "\n";
}

// ---- STATYSTYKI ----
void generateAndShowOrExportStats(const std::vector<Hospital*>& hospitals, bool exportToFile = false) {
    std::ostringstream out;

    int totalPatients = 0, totalChildren = 0, totalAdults = 0;
    double totalAge = 0;
    int totalDoctors = 0;
    std::map<std::string, int> regionPatients;
    std::map<std::string, int> regionChildren;
    std::map<std::string, int> regionAdults;
    std::map<std::string, double> regionAge;
    std::map<std::string, int> regionDoctors;

    out << "--- Statystyki szpitali i pacjentów (obliczenia równoległe) ---\n";

    #pragma omp parallel for reduction(+:totalPatients, totalChildren, totalAdults, totalAge, totalDoctors)
    for (int i = 0; i < hospitals.size(); ++i) {
        auto *h = hospitals[i];
        int szpitalPatients = 0, szpitalChildren = 0, szpitalAdults = 0, szpitalDoctors = 0;
        double szpitalAge = 0.0;

        for (const auto* d : h->getDoctors()) {
            ++szpitalDoctors;
            const auto& pList = d->getPatients();
            szpitalPatients += pList.size();
            for (const auto* p : pList) {
                szpitalAge += p->getAge();
                if (p->getAge() < 18) szpitalChildren++;
                else szpitalAdults++;
            }
        }

        // Zbiorcze statystyki globalne
        totalPatients += szpitalPatients;
        totalChildren += szpitalChildren;
        totalAdults += szpitalAdults;
        totalAge += szpitalAge;
        totalDoctors += szpitalDoctors;

        // Per region
        #pragma omp critical
        {
            regionPatients[h->getRegion()] += szpitalPatients;
            regionChildren[h->getRegion()] += szpitalChildren;
            regionAdults[h->getRegion()] += szpitalAdults;
            regionAge[h->getRegion()] += szpitalAge;
            regionDoctors[h->getRegion()] += szpitalDoctors;

            // Wypisz statystyki dla szpitala
            out << "Szpital: " << h->getName() << " (" << h->getRegion() << ")\n";
            out << "  Lekarzy: " << szpitalDoctors << "\n";
            out << "  Pacjentów: " << szpitalPatients << "\n";
            out << "  Dzieci: " << szpitalChildren << ", Dorośli: " << szpitalAdults << "\n";
            if (szpitalPatients > 0)
                out << "  Śr. wiek pacjenta: " << szpitalAge / szpitalPatients << "\n";
        }
    }

    out << "\n--- Statystyki globalne ---\n";
    out << "Łącznie szpitali: " << hospitals.size() << "\n";
    out << "Łącznie lekarzy: " << totalDoctors << "\n";
    out << "Łącznie pacjentów: " << totalPatients << "\n";
    out << "Łącznie dzieci: " << totalChildren << ", dorośli: " << totalAdults << "\n";
    if (totalPatients > 0)
        out << "Średni wiek pacjenta (wszyscy): " << totalAge / totalPatients << "\n";
    if (totalDoctors > 0)
        out << "Średnio pacjentów na lekarza: " << (double)totalPatients / totalDoctors << "\n";
    if (hospitals.size() > 0)
        out << "Średnio pacjentów na szpital: " << (double)totalPatients / hospitals.size() << "\n";

    out << "\n--- Statystyki wg województw ---\n";
    for (const auto& [region, count] : regionPatients) {
        out << region << ":\n";
        out << "  Lekarzy: " << regionDoctors[region] << "\n";
        out << "  Pacjentów: " << count << "\n";
        out << "  Dzieci: " << regionChildren[region] << ", Dorośli: " << regionAdults[region] << "\n";
        if (count > 0)
            out << "  Śr. wiek pacjenta: " << regionAge[region] / count << "\n";
    }

    if (exportToFile) {
        std::string filename;
        std::cout << "Podaj nazwę pliku do eksportu (domyślnie: statystyki.txt): ";
        std::getline(std::cin, filename);
        if (filename.empty()) filename = "statystyki.txt";
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cout << "Nie można otworzyć pliku!\n";
            return;
        }
        ofs << out.str();
        ofs.close();
        std::cout << "Statystyki wyeksportowano do pliku: " << filename << "\n";
    } else {
        std::cout << out.str();
    }
}

// ---- MENU ----
void displayMenu() {
    std::cout <<
        "\n--- Menu zarządzania szpitalem ---\n"
        "1. Wczytaj lekarzy, pacjentów lub objawy\n"
        "2. Dodaj element (szpital, lekarz, pacjent, objaw)\n"
        "3. Wyświetl dane (szpitale/lekarze, pacjenci, objawy)\n"
        "4. Przypisz pacjentów do lekarzy\n"
        "5. Wyświetl pacjentów przypisanych do lekarzy (posortowane)\n"
        "6. Utwórz kolejki\n"
        "7. Wyświetl kolejki wszystkich lekarzy (posortowane)\n"
        "8. Wyświetl kolejkę lekarza\n"
        "9. Eksportuj kolejki do pliku TXT\n"
        "10. Statystyki (równoległe obliczenia/statystyki)\n"
        "11. Zakończ program\n"
        "Wybierz opcję: ";
}

int main() {
    std::locale::global(std::locale(""));
    std::vector<Hospital*> hospitals;
    std::vector<Patient*> patients;
    std::unordered_map<std::string, std::vector<std::string>> symptomsMap;

    int choice = 0;
    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: {
                int dataChoice;
                std::cout << "Co chcesz wczytać?\n";
                std::cout << "1. Lekarzy\n2. Pacjentów\n3. Objawy\n4. Wszystko z domyślnych plików\n";
                std::cout << "Wybierz opcję: ";
                std::cin >> dataChoice;
                std::cin.ignore();

                switch (dataChoice) {
                    case 1: { // Lekarze
                        std::string filePath;
                        std::cout << "Wczytaj lekarzy z:\n";
                        std::cout << "1. Domyślny plik (doctors.txt)\n";
                        std::cout << "2. Własny plik\n";
                        std::cout << "Wybierz opcję: ";
                        int fileChoice;
                        std::cin >> fileChoice;
                        std::cin.ignore();
                        if (fileChoice == 1) {
                            filePath = "doctors.txt";
                        } else {
                            std::cout << "Podaj ścieżkę do pliku z lekarzami: ";
                            std::getline(std::cin, filePath);
                        }
                        FileHandler::loadDoctors(filePath, hospitals);
                        break;
                    }
                    case 2: { // Pacjenci
                        std::string filePath;
                        std::cout << "Wczytaj pacjentów z:\n";
                        std::cout << "1. Domyślny plik (patients.txt)\n";
                        std::cout << "2. Własny plik\n";
                        std::cout << "Wybierz opcję: ";
                        int fileChoice;
                        std::cin >> fileChoice;
                        std::cin.ignore();
                        if (fileChoice == 1) {
                            filePath = "patients.txt";
                        } else {
                            std::cout << "Podaj ścieżkę do pliku z pacjentami: ";
                            std::getline(std::cin, filePath);
                        }
                        FileHandler::loadPatients(filePath, patients);
                        break;
                    }
                    case 3: { // Objawy
                        std::string filePath;
                        std::cout << "Wczytaj objawy z:\n";
                        std::cout << "1. Domyślny plik (symptoms.txt)\n";
                        std::cout << "2. Własny plik\n";
                        std::cout << "Wybierz opcję: ";
                        int fileChoice;
                        std::cin >> fileChoice;
                        std::cin.ignore();
                        if (fileChoice == 1) {
                            filePath = "symptoms.txt";
                        } else {
                            std::cout << "Podaj ścieżkę do pliku z objawami: ";
                            std::getline(std::cin, filePath);
                        }
                        FileHandler::loadSymptoms(filePath, symptomsMap);
                        break;
                    }
                    case 4: { // Wszystko z domyślnych plików
                        FileHandler::loadDoctors("doctors.txt", hospitals);
                        FileHandler::loadPatients("patients.txt", patients);
                        FileHandler::loadSymptoms("symptoms.txt", symptomsMap);
                        std::cout << "Wczytano lekarzy, pacjentów i objawy z domyślnych plików.\n";
                        break;
                    }
                    default:
                        std::cout << "Nieprawidłowa opcja!\n";
                }
                break;
            }
            case 2: {
                int addChoice;
                std::cout << "Co chcesz dodać?\n";
                std::cout << "1. Szpital\n2. Lekarza\n3. Pacjenta\n4. Objaw\n";
                std::cout << "Wybierz opcję: ";
                std::cin >> addChoice;
                std::cin.ignore();

                switch (addChoice) {
                    case 1: { // Szpital
                        int widx = selectWojewodztwo();
                        if (widx == -1) break;
                        std::string name;
                        std::cout << "Nazwa szpitala: ";
                        std::getline(std::cin, name);
                        hospitals.push_back(new Hospital(name, wojewodztwa[widx]));
                        break;
                    }
                    case 2: { // Lekarz
                        int widx = selectWojewodztwo();
                        if (widx == -1) break;
                        int hidx = selectHospital(hospitals, wojewodztwa[widx]);
                        if (hidx == -1) break;
                        Hospital* hospital = hospitals[hidx];
                        std::string firstName, lastName, specialization;
                        std::cout << "Imię lekarza: "; std::cin >> firstName;
                        std::cout << "Nazwisko lekarza: "; std::cin >> lastName;
                        std::cout << "Specjalizacja: "; std::cin >> specialization;
                        hospital->addDoctor(new Doctor(firstName, lastName, specialization));
                        std::cin.ignore();
                        break;
                    }
                    case 3: { // Pacjent
                        int widx = selectWojewodztwo();
                        if (widx == -1) break;
                        std::string firstName, lastName, region = wojewodztwa[widx], problem;
                        int age;
                        std::cout << "Imię pacjenta: "; std::cin >> firstName;
                        std::cout << "Nazwisko pacjenta: "; std::cin >> lastName;
                        std::cout << "Wiek: "; std::cin >> age;
                        std::cout << "Problem: "; std::cin.ignore(); std::getline(std::cin, problem);

                        if (age < 18) {
                            patients.push_back(new Child(firstName, lastName, age, region, problem));
                        } else {
                            patients.push_back(new Adult(firstName, lastName, age, region, problem));
                        }
                        break;
                    }
                    case 4: { // Objaw
                        std::string specialist, symptom;
                        std::cout << "Specjalista: "; std::cin >> specialist;
                        std::cout << "Objaw: "; std::cin >> symptom;
                        symptomsMap[specialist].push_back(symptom);
                        break;
                    }
                    default:
                        std::cout << "Nieprawidłowa opcja!\n";
                }
                break;
            }
            case 3: {
                int viewChoice;
                std::cout << "Co chcesz wyświetlić?\n";
                std::cout << "1. Szpitale i przypisanych lekarzy (posortowane)\n";
                std::cout << "2. Pacjentów\n";
                std::cout << "3. Objawy\n";
                std::cout << "Wybierz opcję: ";
                std::cin >> viewChoice;
                std::cin.ignore();

                switch (viewChoice) {
                    case 1: {
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
                    case 2: {
                        for(const auto* p : patients) p->printDetails();
                        break;
                    }
                    case 3: {
                        for(const auto& [spec, symptoms] : symptomsMap) {
                            std::cout << spec << ": ";
                            for(const auto& s : symptoms) std::cout << s << ", ";
                            std::cout << "\n";
                        }
                        break;
                    }
                    default:
                        std::cout << "Nieprawidłowa opcja!\n";
                }
                break;
            }
            case 4: {
                assignPatientsBySymptomAndLoad(hospitals, patients, symptomsMap);
                break;
            }
            case 5: {
                std::vector<Hospital*> sortedHospitals = hospitals;
                std::sort(sortedHospitals.begin(), sortedHospitals.end(), [](const Hospital* a, const Hospital* b) {
                    if (a->getRegion() == b->getRegion())
                        return a->getName() < b->getName();
                    return a->getRegion() < b->getRegion();
                });

                for(const auto* h : sortedHospitals) {
                    std::cout << "Szpital: " << h->getName() << " (" << h->getRegion() << ")\n";
                    for(const auto* d : h->getDoctors()) d->printPatients();
                }
                break;
            }
            case 6: {
                assignVisitDates(hospitals);
                break;
            }
            case 7: {
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
                        std::cout << "Brak pacjentów w szpitalu: "
                                  << h->getName() << "\n";
                        continue;
                    }
                    std::cout << "Szpital: "
                              << h->getName() << " (" << h->getRegion() << ")\n";
                    for(const auto* d : h->getDoctors()) d->printQueue();
                }
                break;
            }
            case 8: {
                int widx = selectWojewodztwo();
                if (widx == -1) break;
                int hidx = selectHospital(hospitals, wojewodztwa[widx]);
                if (hidx == -1) break;
                Hospital* hospital = hospitals[hidx];
                int didx = selectDoctor(hospital->getDoctors());
                if (didx == -1) break;
                hospital->getDoctors()[didx]->printQueue();
                break;
            }
            case 9: {
                exportQueuesToTxt(hospitals);
                break;
            }
            case 10: {
                int statOpt = 0;
                std::cout << "1. Wyświetl statystyki na ekranie\n";
                std::cout << "2. Eksportuj statystyki do pliku TXT\n";
                std::cout << "Wybierz opcję: ";
                std::cin >> statOpt;
                std::cin.ignore();
                generateAndShowOrExportStats(hospitals, statOpt == 2);
                break;
            }
            case 11: {
                std::cout << "Program zakończony!\n";
                break;
            }
            default:
                std::cout << "Nieprawidłowa opcja!\n";
        }
    } while(choice != 11);

    for(auto* h : hospitals) delete h;
    for(auto* p : patients) delete p;
    return 0;
}