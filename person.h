#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

class Person {
protected:
    std::string firstName;
    std::string lastName;

public:
    Person() : firstName(""), lastName("") {}
    Person(const std::string& fn, const std::string& ln) : firstName(fn), lastName(ln) {}
    Person(const Person& other) : firstName(other.firstName), lastName(other.lastName) {}
    virtual ~Person() {}

    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }

    virtual void printDetails() const = 0; // Klasa abstrakcyjna
};

#endif // PERSON_H