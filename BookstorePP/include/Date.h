#pragma once
#include <string>

class Date {
public:
    int day, month, year;
    Date();
    std::string to_string() const;
};