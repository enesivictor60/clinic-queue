#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>

struct Patient {
    int id;
    std::string name;
    int urgency;
    std::string arrivalTime;

    static Patient fromLine(const std::string& line) {
        Patient p;
        std::string temp = line;

        size_t pos = temp.find(',');
        p.id = std::stoi(temp.substr(0, pos));
        temp = temp.substr(pos + 1);

        pos = temp.find(',');
        p.name = temp.substr(0, pos);
        temp = temp.substr(pos + 1);

        pos = temp.find(',');
        p.urgency = std::stoi(temp.substr(0, pos));
        temp = temp.substr(pos + 1);

        p.arrivalTime = temp;

        return p;
    }

    std::string toLine() const {
        return std::to_string(id) + "," + name + "," + std::to_string(urgency) + "," + arrivalTime;
    }
};

#endif
