#include <iostream>
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv

int main() {
    // TODO: Get the current date

    // Get the BIRTHDATE environment variable, so that we can
    // congratulate the user if the current date matches that.
    const char *birthdateEnv = std::getenv("BIRTHDATE");
    if (nullptr != birthdateEnv) {  // the BIRTHDATE environment variable exists
        std::string birthdateString = birthdateEnv;  // assignment operator makes a string out of char *
        std::cout << "BIRTHDATE = " << birthdateString << std::endl;  // TODO: remove this

        // TODO: Parse the YYYY-MM-DD string, or bail out if the format is wrong

        // TODO: If the birthdate matches the current date, congratulate the user
    }
    else {
        std::cout << "No BIRTHDATE environment variable found" << std::endl;  // TODO: remove this
    }

    return 0;
}
