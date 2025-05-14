#ifndef HOSPITAL_SYSTEM_H
#define HOSPITAL_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Patient.h"
#include "Doctor.h"
#include "AppointmentScheduler.h"

class HospitalSystem {
private:
    std::vector<std::shared_ptr<Patient>> patients;
    std::map<std::string, std::vector<std::shared_ptr<Doctor>>> doctorsByRegion;
    std::map<std::string, std::vector<std::string>> symptomsToSpecialists;

public:
    void loadDoctors(const std::string& filename);
    void loadSymptoms(const std::string& filename);
    void loadPatients(const std::string& filename);
    void addPatient();
    void assignPatientsToDoctors();
    void scheduleAppointments();
    void printPatients() const;
};

#endif