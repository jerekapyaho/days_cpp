#pragma once

#include <string>
#include <chrono>

class Event {
public:
    Event(
        const std::chrono::year_month_day& t, 
        const std::string& c, 
        const std::string& d) :
            timestamp(t), category(c), description(d) {

    }

    std::chrono::year_month_day getTimestamp() const;
    std::string getCategory() const;
    std::string getDescription() const;

private:
    std::chrono::year_month_day timestamp;
    std::string category;
    std::string description;
};
