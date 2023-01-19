#include <iostream>
#include <iomanip>  // for output stream control
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
    using namespace std;  // use std facilities without prefix inside this function

    constexpr string_view yyyymmdd = "YYYY-MM-DD";
    if (buf.size() != yyyymmdd.size()) {
        return nullopt;
    }

    istringstream input(buf);
    string part;
    vector<string> parts;
    while (getline(input, part, '-')) {
        parts.push_back(part);
    }
    if (parts.size() != 3) {  // expecting three components, year-month-day
        return nullopt;
    }

    int year{0};
    unsigned int month{0};
    unsigned int day{0};
    try {
        year = stoul(parts.at(0));
        month = stoi(parts.at(1));
        day = stoi(parts.at(2));

        return chrono::year_month_day{
            chrono::year{year},
            chrono::month(month),
            chrono::day(day)};
    }
    catch (invalid_argument const& ex) {
        cerr << "conversion error: " << ex.what() << endl;
    }
    catch (out_of_range const& ex) {
        cerr << "conversion error: " << ex.what() << endl;
    }

    return nullopt;
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

// Returns `date` as a string in `YYYY-MM-DD` format.
// The ostream support for std::chrono::year_month_day is not
// available in most (any?) compilers, so we roll our own.  
std::string getStringFromDate(const std::chrono::year_month_day& date) {
    std::ostringstream result;

    result 
        << std::setfill('0') << std::setw(4) << static_cast<int>(date.year())
        << "-" << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.month())
        << "-" << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.day());

    return result.str();
}

// Helper to print an event to standard output
void displayEvent(const Event& event) {
    std::cout 
        << getStringFromDate(event.getTimestamp()) << ": " 
        << event.getDescription() << '\n'; 
}

int main() {
    using namespace std;

    // Get the current date from the system clock and extract year_month_day.
    // See https://en.cppreference.com/w/cpp/chrono/year_month_day
    const chrono::time_point now = chrono::system_clock::now();
    const chrono::year_month_day currentDate{chrono::floor<chrono::days>(now)};

    // Check the birthdate and user with generic helper functions
    auto birthdateValue = getEnvironmentVariable("BIRTHDATE");
    if (birthdateValue.has_value()) {
        auto birthdate = getDateFromString(birthdateValue.value());
        if (birthdate.has_value()) {
            auto b = birthdate.value();
            if (b.month() == currentDate.month() && b.day() == currentDate.day()) {
                ostringstream message;
                message << "Happy birthday";
                auto userEnv = getEnvironmentVariable("USER");
                if (userEnv.has_value()) {
                    auto user = userEnv.value();
                    message << ", " << user;
                }
                message << "!\n";
                cout << message.str();
            }
        }
    }

    // Add a few events for testing, then print them out:

    Event ev1(
        getDateFromString("2020-12-15").value(), 
        "computing", 
        "C++20 released");
    Event ev2(
        getDateFromString("2023-01-10").value(), 
        "computing", 
        "Rust 1.66.1 released");
    Event ev3(
        getDateFromString("2022-09-20").value(), 
        "computing", 
        "Java SE 19 released");
    Event ev4(
        getDateFromString("2014-11-12").value(),
        "computing",
        ".NET Core released");

    displayEvent(ev1);
    displayEvent(ev2);
    displayEvent(ev3);
    displayEvent(ev4);

    return 0;
}
