#ifndef APPOINTMENT_SCHEDULER_H
#define APPOINTMENT_SCHEDULER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Patient.h"
#include "Doctor.h"

class AppointmentScheduler {
private:
    std::map<std::string, std::vector<std::shared_ptr<Patient>>> appointments;

public:
    void addAppointment(const std::string& doctorName, const std::shared_ptr<Patient>& patient);
    void printSchedule() const;
};

#endif