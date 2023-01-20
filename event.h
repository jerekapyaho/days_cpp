#pragma once

#include <string>
#include <chrono>

// Represents an event.
class Event {
public:
    Event(
        const std::chrono::year_month_day& t, 
        const std::string& c, 
        const std::string& d) :
            timestamp(t), category(c), description(d) {

    }

    // Getters for the properties:
    std::chrono::year_month_day getTimestamp() const;
    std::string getCategory() const;
    std::string getDescription() const;

    // Overloaded operator for output stream use.
    // Needs to be `friend`, not a method in this class.
    friend std::ostream& operator<<(std::ostream& os, const Event& event);    

private:
    std::chrono::year_month_day timestamp;
    std::string category;
    std::string description;
};
