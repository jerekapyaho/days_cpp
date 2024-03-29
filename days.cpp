#include <iostream> // for standard I/O streams
#include <iomanip>  // for stream control
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv
#include <chrono>   // for the std::chrono facilities
#include <sstream>  // for std::stringstream class
#include <vector>   // for std::vector class
#include <optional> // for std::optional
#include <string_view>  // for std::string_view
#include <filesystem>  // for path utilities
#include <memory>   // for smart pointers

#include "event.h"  // for our Event class
#include "rapidcsv.h"  // for the header-only library RapidCSV

// Parses the string `buf` for a date in YYYY-MM-DD format. If `buf` can be parsed,
// returns a wrapped `std::chrono::year_month_day` instance, otherwise `std::nullopt`.
// NOTE: Once clang++ and g++ implement chrono::from_stream, this could be replaced by
// something like this:
//  chrono::year_month_day birthdate;
//  std::istringstream bds{birthdateValue};
//  std::basic_istream<char> stream{bds.rdbuf()};
//  chrono::from_stream(stream, "%F", birthdate);
// However, I don't know how errors should be handled. Maybe this function could then
// continue to serve as a wrapper.
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

        auto result = chrono::year_month_day{
            chrono::year{year},
            chrono::month(month),
            chrono::day(day)};

        if (result.ok()) {
            return result;
        }
        else {
            return nullopt;
        }
    }
    catch (invalid_argument const& ex) {
        cerr << "conversion error: " << ex.what() << endl;
    }
    catch (out_of_range const& ex) {
        cerr << "conversion error: " << ex.what() << endl;
    }

    return nullopt;
}

// Returns the value of the environment variable `name` as an `std::optional`
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
// The ostream support for `std::chrono::year_month_day` is not
// available in most (any?) compilers, so we roll our own.
std::string getStringFromDate(const std::chrono::year_month_day& date) {
    std::ostringstream result;

    result
        << std::setfill('0') << std::setw(4) << static_cast<int>(date.year())
        << "-" << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.month())
        << "-" << std::setfill('0') << std::setw(2) << static_cast<unsigned>(date.day());

    return result.str();
}


// Print `T` to standard output.
// `T` needs to have an overloaded << operator.
template <typename T>
void display(const T& value) {
    std::cout << value;
}

// Prints a newline to standard output.
inline void newline() {
    std::cout << std::endl;
}

// Overload the << operator for the Event class.
// See https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170
std::ostream& operator <<(std::ostream& os, const Event& event) {
    os
        << getStringFromDate(event.getTimestamp()) << ": "
        << event.getDescription()
        << " (" + event.getCategory() + ")";
    return os;
}

// Gets the number of days between two points in time.
int getNumberOfDaysBetween(std::chrono::sys_days const& earlier, std::chrono::sys_days const& later) {
    return (later - earlier).count();
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
        ostringstream message;
        if (birthdate.has_value()) {
            auto b = birthdate.value();
            if (b.month() == currentDate.month() && b.day() == currentDate.day()) {
                message << "Happy birthday";
                auto userEnv = getEnvironmentVariable("USER");
                if (userEnv.has_value()) {
                    auto user = userEnv.value();
                    message << ", " << user;
                }
                message << "! ";
            }

            int age = getNumberOfDaysBetween(
                chrono::floor<chrono::days>(chrono::sys_days{b}),
                chrono::floor<chrono::days>(chrono::sys_days{currentDate})
            );

            message << "You are " << age << " days old.";
            if (age % 1000 == 0) {
                message << " That's a nice round number!";
            }

            display(message.str()); newline();
        }
    }

    // Note that you can't print an `std::chrono::year_month_day`
    // with `display()` because there is no overloaded << operator
    // for it (yet).

    // Construct a path for the events file.
    // If the user's home directory can't be determined, give up.
    string homeDirectoryString;
    auto homeString = getEnvironmentVariable("HOME");
    if (!homeString.has_value()) {
        // HOME not found, maybe this is Windows? Try USERPROFILE.
        auto userProfileString = getEnvironmentVariable("USERPROFILE");
        if (!userProfileString.has_value()) {
            std::cerr << "Unable to determine home directory";
            return 1;
        }
        else {
            homeDirectoryString = userProfileString.value();
        }
    }
    else {
        homeDirectoryString = homeString.value();
    }

    namespace fs = std::filesystem; // save a little typing
    fs::path daysPath{homeDirectoryString};
    daysPath /= ".days"; // append our own directory
    if (!fs::exists(daysPath)) {
        display(daysPath.string());
        display(" does not exist, please create it");
        newline();
        return 1;  // nothing to do anymore, exit program

        // To create the directory:
        //std::filesystem::create_directory(daysPath);
        // See issue: https://github.com/jerekapyaho/days_cpp/issues/4
    }

    // Now we should have a valid path to the `~/.days` directory.
    // Construct a pathname for the `events.csv` file.
    auto eventsPath = daysPath / "events.csv";

    //
    // Read in the CSV file from `eventsPath` using RapidCSV
    // See https://github.com/d99kris/rapidcsv
    //
    rapidcsv::Document document{eventsPath.string()};
    vector<string> dateStrings{document.GetColumn<string>("date")};
    vector<string> categoryStrings{document.GetColumn<string>("category")};
    vector<string> descriptionStrings{document.GetColumn<string>("description")};

    vector<Event> events;
    for (size_t i{0}; i < dateStrings.size(); i++) {
        auto date = getDateFromString(dateStrings.at(i));
        if (!date.has_value()) {
            cerr << "bad date at row " << i << ": " << dateStrings.at(i) << '\n';
            continue;
        }

        Event event{
            date.value(),
            categoryStrings.at(i),
            descriptionStrings.at(i)
        };
        events.push_back(event);
    }

    const auto today = chrono::sys_days{
        floor<chrono::days>(chrono::system_clock::now())};

    for (auto& event : events) {
        const auto delta = (chrono::sys_days{event.getTimestamp()} - today).count();

        ostringstream line;
        line << event << " - ";

        if (delta < 0) {
            line << abs(delta) << " days ago";
        }
        else if (delta > 0) {
            line << "in " << delta << " days";
        }
        else {
            line << "today";
        }

        display(line.str());
        newline();
    }

    return 0;
}
