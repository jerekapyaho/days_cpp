#include <iostream>
#include <iomanip>  // for controlling stream output
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv
#include <chrono>   // for the std::chrono facilities
#include <sstream>  // for std::stringstream class
#include <vector>   // for std::vector class
#include <optional> // for std::optional

// Returns the value of the `BIRTHDATE` environment variable
// if it exists as an `std::chrono::year_month_day` wrapped
// in an `std::optional`, otherwise returns `std::nullopt`.
std::optional<std::chrono::year_month_day> getBirthdate() {
    const char *birthdateEnv = std::getenv("BIRTHDATE");
    if (nullptr != birthdateEnv) {  // the BIRTHDATE environment variable exists
        std::string birthdateString = birthdateEnv;  // assignment operator makes a string out of char *

        // Parse the YYYY-MM-DD string, or bail out if the format is wrong
        std::istringstream birthdateStringStream(birthdateString);
        std::string segment;
        std::vector<std::string> segmentList;
        while (std::getline(birthdateStringStream, segment, '-')) {
            segmentList.push_back(segment);
        }
        if (segmentList.size() != 3) {  // expecting three components, year-month-day
            return std::nullopt;
        }

        int birthYear{0};
        unsigned int birthMonth{0};
        unsigned int birthDay{0};
        try {
            birthYear = std::stoul(segmentList.at(0));
            birthMonth = std::stoi(segmentList.at(1));
            birthDay = std::stoi(segmentList.at(2));

            return std::chrono::year_month_day{
                std::chrono::year{birthYear},
                std::chrono::month(birthMonth),
                std::chrono::day(birthDay)};
        }
        catch (std::invalid_argument const& ex) {
            std::cerr << "conversion error: " << ex.what() << std::endl;
            return std::nullopt;
        }
        catch (std::out_of_range const& ex) {
            std::cerr << "conversion error: " << ex.what() << std::endl;
            return std::nullopt;
        }
    }

    return std::nullopt;
}

std::optional<std::string> getUser() {
    const char *userEnv = std::getenv("USER");
    if (nullptr != userEnv) {
        std::string userString = userEnv;
        return userString;
    }
    return std::nullopt;
}

int main() {
    // Get the current date from the system clock and extract year_month_day.
    // See https://en.cppreference.com/w/cpp/chrono/year_month_day
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::year_month_day currentDate{std::chrono::floor<std::chrono::days>(now)};

    // Compact way of checking the birthdate and user, using functions
    // that return `std::optional` objects:
    auto birthdate = getBirthdate();
    if (birthdate.has_value()) {
        auto b = birthdate.value();
        if (b.month() == currentDate.month() && b.day() == currentDate.day()) {
            std::cout << "Happy birthday";
            auto userEnv = getUser();
            if (userEnv.has_value()) {
                auto user = userEnv.value();
                std::cout << ", " << user;
            }
            std::cout << "!\n";
        }
    }

    return 0;
}
