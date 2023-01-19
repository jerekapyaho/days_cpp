#pragma once

#include <string>

class Event {
public:
    Event(
        const std::string& t, 
        const std::string& c, 
        const std::string& d) : 
        timestamp(t), category(c), description(d) {

    }

    std::string timestamp;
    std::string category;
    std::string description;
};
