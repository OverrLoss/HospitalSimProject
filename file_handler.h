#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "hospital.h"
#include "patient.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

class FileHandler {
public:
    static void loadDoctors(const std::string& filePath, std::vector<Hospital*>& hospitals) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        std::string line;
        std::string currentRegion;
        Hospital* currentHospital = nullptr;

        while (std::getline(file, line)) {
            line = trim(line);

            if (line == "[Region]") {
                if (std::getline(file, line)) {
                    currentRegion = trim(line);
                }
            } else if (line == "[Hospital]") {
                if (std::getline(file, line)) {
                    std::string hospitalName = trim(line);
                    currentHospital = new Hospital(hospitalName, currentRegion);
                    hospitals.push_back(currentHospital);
                }
            } else if (line == "[Doctors]") {
                while (std::getline(file, line) && !line.empty() && line[0] != '[') {
                    std::size_t dashPos = line.find(" - ");
                    if (dashPos != std::string::npos) {
                        std::string fullName = line.substr(0, dashPos);
                        std::string specialization = line.substr(dashPos + 3);
                        fullName = trim(fullName);
                        specialization = trim(specialization);

                        std::istringstream nameStream(fullName);
                        std::string firstName, lastName;
                        nameStream >> firstName >> lastName;

                        if (currentHospital != nullptr) {
                            currentHospital->addDoctor(new Doctor(firstName, lastName, specialization));
                        }
                    }
                }
            }
        }
        file.close();
    }

    static void loadSymptoms(const std::string& filePath, std::unordered_map<std::string, std::vector<std::string>>& symptomsMap) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream stream(line);
            std::string symptom, disease;
            if (std::getline(stream, symptom, ':') && std::getline(stream, disease)) {
                symptomsMap[symptom].push_back(disease);
            }
        }
        file.close();
    }

    static void loadPatients(const std::string& filePath, std::vector<Patient*>& patients) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream stream(line);
            std::string firstName, lastName, region, problem;
            int age;

            if (stream >> firstName >> lastName >> age >> region) {
                std::getline(stream, problem); // Pobranie pozostałego tekstu jako problem
                Patient* patient = new Patient(firstName, lastName, age, region, problem);
                patients.push_back(patient);
            }
        }
        file.close();
    }

private:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
    }
};

#endif // FILE_HANDLER_H