#include "event.h"

std::string Event::getTimestamp() const {
    return timestamp;
}

std::string Event::getCategory() const {
    return category;
}

std::string Event::getDescription() const {
    return description;
}
