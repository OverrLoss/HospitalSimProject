#include "HospitalSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>

void HospitalSystem::loadDoctors(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return;
    }

    std::string line;
    std::string currentRegion;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Check if it's a region line
        if (line.find(" - ") != std::string::npos) {
            currentRegion = line.substr(0, line.find(" - "));
            doctorsByRegion[currentRegion] = {};
        } else {
            // Parse doctor details
            std::istringstream iss(line);
            std::string name, specialization;
            std::getline(iss, name, '-');  // Extract name
            std::getline(iss, specialization);  // Extract specialization

            // Trim leading/trailing whitespace
            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            specialization.erase(0, specialization.find_first_not_of(" \t"));
            specialization.erase(specialization.find_last_not_of(" \t") + 1);

            // Create and store doctor
            auto doctor = std::make_shared<Doctor>(name, specialization);
            doctorsByRegion[currentRegion].push_back(doctor);
        }
    }
    file.close();
}