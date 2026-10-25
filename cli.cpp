//
// Created by макар on 25.10.2026.
//

#include "cli.h"
#include <string>
#include <stdexcept>

int parse_movie_id(const std :: string & value) {
    std :: size_t pos = 0;

    int id = std :: stoi(value, &pos);

    if (pos != value.length() || id <= 0) {
        throw std::runtime_error("Invalid Movie ID.");
    }

    return id;
}