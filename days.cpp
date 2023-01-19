#include <iostream>
#include <iomanip>  // for controlling stream output
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv
#include <chrono>   // for the std::chrono facilities
#include <sstream>  // for std::stringstream class
#include <vector>   // for std::vector class
#include <optional> // for std::optional
#include <string_view>  // for std::string_view

#include "event.h"  // for our Event class

// Parses the string `buf` for a date in YYYY-MM-DD format. If `buf` can be parsed,
// returns a wrapped `std::chrono::year_month_day` instances, otherwise `std::nullopt`.
std::optional<std::chrono::year_month_day> getDateFromString(const std::string& buf) {
    constexpr std::string_view yyyymmdd = "YYYY-MM-DD";
    if (buf.size() != yyyymmdd.size()) {
        return std::nullopt;
    }

    std::istringstream input(buf);
    std::string part;
    std::vector<std::string> parts;
    while (std::getline(input, part, '-')) {
        parts.push_back(part);
    }
    if (parts.size() != 3) {  // expecting three components, year-month-day
        return std::nullopt;
    }

    int year{0};
    unsigned int month{0};
    unsigned int day{0};
    try {
        year = std::stoul(parts.at(0));
        month = std::stoi(parts.at(1));
        day = std::stoi(parts.at(2));

        return std::chrono::year_month_day{
            std::chrono::year{year},
            std::chrono::month(month),
            std::chrono::day(day)};
    }
    catch (std::invalid_argument const& ex) {
        std::cerr << "conversion error: " << ex.what() << std::endl;
        return std::nullopt;
    }
    catch (std::out_of_range const& ex) {
        std::cerr << "conversion error: " << ex.what() << std::endl;
        return std::nullopt;
    }

    return std::nullopt;
}

// Returns the value of the environment variable `name` as an `std::optional``
// value. If the variable exists, the value is a wrapped `std::string`,
// otherwise `std::nullopt`.
std::optional<std::string> getEnvironmentVariable(const std::string& name) {
    const char *value = std::getenv(const_cast<char *>(name.c_str()));
    if (nullptr != value) {
        std::string valueString = value;
        return valueString;
    }
    return std::nullopt;
}

int main() {
    // Get the current date from the system clock and extract year_month_day.
    // See https://en.cppreference.com/w/cpp/chrono/year_month_day
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::year_month_day currentDate{std::chrono::floor<std::chrono::days>(now)};

    // Check the birthdate and user with generic helper functions
    auto birthdateValue = getEnvironmentVariable("BIRTHDATE");
    if (birthdateValue.has_value()) {
        auto birthdate = getDateFromString(birthdateValue.value());
        if (birthdate.has_value()) {
            auto b = birthdate.value();
            if (b.month() == currentDate.month() && b.day() == currentDate.day()) {
                std::ostringstream message;
                message << "Happy birthday";
                auto userEnv = getEnvironmentVariable("USER");
                if (userEnv.has_value()) {
                    auto user = userEnv.value();
                    message << ", " << user;
                }
                message << "!\n";
                std::cout << message.str();
            }
        }
    }

    Event ev1(
        "1989-11-09", 
        "history", 
        "Fall of the Berlin wall");
    Event ev2(
        "1995-01-01", 
        "history", 
        "Finland joined the European Union");
    Event ev3("2010-10-01", "economy", "Something big");

    std::cout << ev1.getTimestamp() << ": " << ev1.getDescription() << '\n';
    std::cout << ev2.getTimestamp() << ": " << ev2.getDescription() << '\n';
    std::cout << ev3.getTimestamp() << ": " << ev3.getDescription() << '\n';

    return 0;
}










