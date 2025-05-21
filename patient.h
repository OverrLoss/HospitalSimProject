#ifndef PATIENT_H
#define PATIENT_H

#include "person.h"
#include <string>
#include <iostream>

// Bazowa klasa Pacjent
class Patient : public Person {
protected:
    int age;
    std::string region;
    std::string problem;
    std::string visitDateTime; // Data i godzina wizyty

public:
    Patient() : Person(), age(0), region(""), problem("") {}
    Patient(const std::string& firstName, const std::string& lastName, int age, const std::string& region, const std::string& problem)
        : Person(firstName, lastName), age(age), region(region), problem(problem) {}
    Patient(const Patient& other)
        : Person(other), age(other.age), region(other.region), problem(other.problem), visitDateTime(other.visitDateTime) {}
    ~Patient() override {}

    virtual std::string getType() const { return "Pacjent"; }

    int getAge() const { return age; }
    const std::string& getRegion() const { return region; }
    const std::string& getProblem() const { return problem; }

    void setVisitDateTime(const std::string& dt) { visitDateTime = dt; }
    const std::string& getVisitDateTime() const { return visitDateTime; }

    virtual void printDetails() const override {
        std::cout << getType() << ": " << firstName << " " << lastName
                  << ", Wiek: " << age
                  << ", Województwo: " << region
                  << ", Problem: " << problem
                  << (visitDateTime.empty() ? "" : (", Wizyta: " + visitDateTime))
                  << std::endl;
    }
};

// Klasa Dziecko
class Child : public Patient {
public:
    Child(const std::string& firstName, const std::string& lastName, int age, const std::string& region, const std::string& problem)
        : Patient(firstName, lastName, age, region, problem) {}
    std::string getType() const override { return "Dziecko"; }
    void printDetails() const override {
        std::cout << getType() << ": " << firstName << " " << lastName
                  << ", Wiek: " << age
                  << ", Województwo: " << region
                  << ", Problem: " << problem
                  << (visitDateTime.empty() ? "" : (", Wizyta: " + visitDateTime))
                  << std::endl;
    }
};

// Klasa Dorosły
class Adult : public Patient {
public:
    Adult(const std::string& firstName, const std::string& lastName, int age, const std::string& region, const std::string& problem)
        : Patient(firstName, lastName, age, region, problem) {}
    std::string getType() const override { return "Dorosły"; }
    void printDetails() const override {
        std::cout << getType() << ": " << firstName << " " << lastName
                  << ", Wiek: " << age
                  << ", Województwo: " << region
                  << ", Problem: " << problem
                  << (visitDateTime.empty() ? "" : (", Wizyta: " + visitDateTime))
                  << std::endl;
    }
};

#endif // PATIENT_H