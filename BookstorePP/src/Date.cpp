#include "Date.h"
#include <ctime>

Date::Date() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    day = now->tm_mday;
    month = now->tm_mon + 1;
    year = now->tm_year + 1900;
}

std::string Date::to_string() const {
    return std::to_string(day) + " " + std::to_string(month) + " " + std::to_string(year);
}