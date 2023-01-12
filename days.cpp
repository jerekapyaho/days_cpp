#include <iostream>
#include <string>   // for std::string class
#include <cstdlib>  // for std::getenv
#include <filesystem>  // for std::filesystem (since C++17)

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <pwd.h>  // for getpwuid()
#include <unistd.h> // for getuid()
#endif

// Returns the user's home directory, or empty if it can't be determined.
std::string getHome() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // TODO: What about the %APPDATA% environment variable?

    // For Windows, try the USERPROFILE environment variable
    const char *userProfileEnv = std::getenv("USERPROFILE");
    if (nullptr != userProfileEnv) {
        std::string userProfileString = userProfileEnv;
        return
        userProfileString.append("\\AppData");
        userProfileString.append("days");
    }
    else {
        const char *homeDriveEnv = std::getenv("HOMEDRIVE");
        const char *homePathEnv = std::getenv("HOMEPATH");

        // Concatenate HOMEDRIVE and HOMEPATH, with a backslash between them
        std::string homeDriveString = homeDriveEnv;
        homeDriveString.append("\\");
        homeDriveString.append(homePathEnv);

        // Concatenate the `.days` part
        homeDriveString.append("\\");
        homeDriveString.append(".days");
    }

#else if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    // Linux and other UNIXes most likely have HOME, macOS might have it
    const char *homeEnv = std::getenv("HOME");
    if (nullptr != homeEnv) {  // the HOME environment variable exists
        return std::string(homeEnv);
    }
    else { // try to get the password directory
        struct passwd *pw = getpwuid(getuid());
        if (nullptr != pw) {
            return std::string(pw->pw_dir);
        }
        else {  // give up and return the empty string
            return std::string("");
        }
    }
#endif
}

// Creates the `~/.days` directory if it doesn't already exist.
bool createDaysDirIfNecessary() {
    // Get the user's home directory, if possible
    auto daysPathString = getHome();
    if (daysPathString.empty()) {  // bail out
        std::cerr << "Unable to determine home directory" << std::endl;
        return -1;
    }
    else {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        daysPathString.append("\\AppData\\");
#else
        daysPathString.append("/.config/days");
#endif
    }

    std::cout << "daysPathString = " << daysPathString << std::endl;

    const std::filesystem::path daysPath{daysPathString};
    if (!std::filesystem::exists(daysPath)) {  // does not yet exist, need to create
        std::filesystem::create_directory(daysPath);
        return true;  // true means the directory was created
    }

    std::cout << "daysPath exists, no need to create" << std::endl;

    return false;  // nothing needed to be done, return false
}

int main() {
    // If the `~/.days` directory does not exist, create it
    createDaysDirIfNecessary();

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
