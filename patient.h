#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream> // Dodano nagłówek umożliwiający korzystanie z cout i endl

class Patient {
private:
    std::string firstName;
    std::string lastName;
    int age;
    std::string region;
    std::string problem;

public:
    // Konstruktor
    Patient(const std::string& firstName, const std::string& lastName, int age, const std::string& region, const std::string& problem)
        : firstName(firstName), lastName(lastName), age(age), region(region), problem(problem) {}

    // Getter dla imienia
    const std::string& getFirstName() const {
        return firstName;
    }

    // Getter dla nazwiska
    const std::string& getLastName() const {
        return lastName;
    }

    // Getter dla wieku
    int getAge() const {
        return age;
    }

    // Getter dla regionu
    const std::string& getRegion() const {
        return region;
    }

    // Getter dla problemu
    const std::string& getProblem() const {
        return problem;
    }

    // Funkcja wyświetlająca szczegóły pacjenta
    void printDetails() const {
        std::cout << "Patient: " << firstName << " " << lastName
                  << ", Age: " << age
                  << ", Region: " << region
                  << ", Problem: " << problem << std::endl;
    }
};

#endif // PATIENT_H