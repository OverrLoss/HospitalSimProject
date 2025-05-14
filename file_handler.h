#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "hospital.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class FileHandler {
public:
    // Wczytywanie lekarzy z pliku
    static void loadDoctors(const std::string& filePath, std::vector<Hospital*>& hospitals) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        std::string line;
        Hospital* currentHospital = nullptr;
        while (std::getline(file, line)) {
            // Jeśli linia zawiera nazwę szpitala
            if (line.find(" - ") != std::string::npos) {
                std::istringstream iss(line);
                std::string region, hospitalName;
                std::getline(iss, region, '-');
                std::getline(iss, hospitalName);

                // Usunięcie zbędnych spacji
                region = trim(region);
                hospitalName = trim(hospitalName);

                currentHospital = new Hospital(hospitalName, region);
                hospitals.push_back(currentHospital);
            } else if (currentHospital != nullptr && !line.empty()) {
                // Jeśli linia zawiera lekarza
                std::istringstream iss(line);
                std::string firstName, lastName, specialization;
                iss >> firstName >> lastName;
                std::getline(iss, specialization, '-');
                specialization = trim(specialization);

                currentHospital->addDoctor(firstName, lastName, specialization);
            }
        }

        file.close();
    }

    // Wczytywanie symptomów z pliku
    static void loadSymptoms(const std::string& filePath, std::unordered_map<std::string, std::vector<std::string>>& symptomsMap) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Jeśli linia zawiera specjalistę i jego symptomy
            if (line.find(":") != std::string::npos) {
                std::istringstream iss(line);
                std::string specialist, symptoms;
                std::getline(iss, specialist, ':');
                std::getline(iss, symptoms);

                specialist = trim(specialist);
                std::vector<std::string> symptomList = split(symptoms, ',');
                symptomsMap[specialist] = symptomList;
            }
        }

        file.close();
    }

    // Wczytywanie pacjentów z pliku
    static void loadPatients(const std::string& filePath, std::vector<Patient*>& patients) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string firstName, lastName, region, problem;
            int age;

            if (iss >> firstName >> lastName >> age >> region) {
                std::getline(iss, problem);
                problem = trim(problem);

                Patient* newPatient = new Patient(firstName, lastName, age, region, problem);
                patients.push_back(newPatient);
            }
        }

        file.close();
    }

private:
    // Funkcja pomocnicza do usuwania zbędnych spacji
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    // Funkcja pomocnicza do dzielenia stringa
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::istringstream iss(str);
        std::string token;
        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }
};

#endif // FILE_HANDLER_H