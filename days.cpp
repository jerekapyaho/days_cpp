#include <iostream>
#include <iomanip>  // for controlling stream output
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv
#include <chrono>   // for the std::chrono facilities
#include <sstream>  // for std::stringstream class
#include <vector>   // for std::vector class

int main() {
    // Get the current date from the system clock and extract year_month_day.
    // See https://en.cppreference.com/w/cpp/chrono/year_month_day
    const std::chrono::time_point now = std::chrono::system_clock::now();
    const std::chrono::year_month_day currentDate{std::chrono::floor<std::chrono::days>(now)};

    // Print out the current date just for checking.
    // Note that the values need to be cast before they can be printed.
    // Use output manipulators to get ISO standard two-digit months and days.
    std::cout << "Current date = "
        << static_cast<int>(currentDate.year()) << "-"
        << std::setw(2) << std::setfill('0') << static_cast<unsigned>(currentDate.month()) << "-"
        << std::setw(2) << std::setfill('0') << static_cast<unsigned>(currentDate.day())
        << std::endl;

    // Get the BIRTHDATE environment variable, so that we can
    // congratulate the user if the current date matches that.
    const char *birthdateEnv = std::getenv("BIRTHDATE");
    if (nullptr != birthdateEnv) {  // the BIRTHDATE environment variable exists
        std::string birthdateString = birthdateEnv;  // assignment operator makes a string out of char *
        std::cout << "BIRTHDATE = " << birthdateString << std::endl;  // TODO: remove this

        // Parse the YYYY-MM-DD string, or skip this feature if the format is wrong
        std::istringstream birthdateStringStream(birthdateString);
        std::string segment;
        std::vector<std::string> segmentList;
        while (std::getline(birthdateStringStream, segment, '-')) {
            segmentList.push_back(segment);
        }
        if (segmentList.size() != 3) {  // expecting exactly three components, YYYY MM DD
            std::cerr << "Value of BIRTHDATE environment variable has invalid format: "
                << birthdateString << std::endl;
        }
        else {
            std::cout
                << "birthdate: "
                << " year = " << segmentList.at(0)
                << " month = " << segmentList.at(1)
                << " day = " << segmentList.at(2)
                << std::endl;

            // If the birthdate matches the current date, congratulate the user
            unsigned int birthMonth{0};
            unsigned int birthDay{0};
            try {
                birthMonth = std::stoi(segmentList.at(1));
                birthDay = std::stoi(segmentList.at(2));

                // If conversion of month or day from string to int failed
                // with an exception, we should still have zeros in both.
                // The current date can't have zero month or day, so the
                // upcoming check produces the correct result.

                // TODO: Remove this after verifying
                std::cout << "parsed: month = " << birthMonth << " day = " << birthDay << std::endl;

                // Check if month and day of birthdate match those of the current date.
                if (birthMonth == static_cast<unsigned>(currentDate.month())
                    && birthDay == static_cast<unsigned>(currentDate.day())) {
                    std::cout << "Happy birthday";

                    // Get the USER environment variable.
                    // If it doesn't exist, congratulate without username.
                    const char *userEnv = std::getenv("USER");
                    if (nullptr != userEnv) {
                        std::string userString = userEnv;

                        std::cout << ", " << userString;
                    }
                    std::cout << "!\n";
                }
            }
            catch (std::invalid_argument const& ex) {
                std::cerr << "conversion error: " << ex.what() << std::endl;
            }
            catch (std::out_of_range const& ex) {
                std::cerr << "conversion error: " << ex.what() << std::endl;
            }
        }
    }
    else {
        std::cout << "No BIRTHDATE environment variable found" << std::endl;  // TODO: remove this
    }

    std::cout << "end program" << std::endl;

    return 0;
}
