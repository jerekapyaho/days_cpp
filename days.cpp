#include <iostream>
#include <iomanip>  // for controlling stream output
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv
#include <chrono>   // for the std::chrono facilities
#include <sstream>  // for std::stringstream class
#include <vector>   // for std::vector class
#include <optional> // for std::optional

// Parses the string `buf` for a date in YYYY-MM-DD format. If `buf` can be parsed,
// returns a wrapped `std::chrono::year_month_day` instances, otherwise `std::nullopt`.
std::optional<std::chrono::year_month_day> getDateFromString(const std::string& buf) {
    std::istringstream input(buf);
    std::string segment;
    std::vector<std::string> segmentList;
    while (std::getline(input, segment, '-')) {
        segmentList.push_back(segment);
    }
    if (segmentList.size() != 3) {  // expecting three components, year-month-day
        return std::nullopt;
    }

    int year{0};
    unsigned int month{0};
    unsigned int day{0};
    try {
        year = std::stoul(segmentList.at(0));
        month = std::stoi(segmentList.at(1));
        day = std::stoi(segmentList.at(2));

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

    return 0;
}
