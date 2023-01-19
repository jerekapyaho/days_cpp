#include "event.h"

std::chrono::year_month_day Event::getTimestamp() const {
    return timestamp;
}

std::string Event::getCategory() const {
    return category;
}

std::string Event::getDescription() const {
    return description;
}
