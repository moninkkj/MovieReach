//
// Created by макар on 25.10.2026.
//

#include <cassert>
#include <iostream>

#include "cli.h"

bool throws_on_invalid_id(const std :: string & value) {
    bool exceptionThrown = false;

    try {
        parse_movie_id(value);
    }
    catch (const std::exception &) {
        exceptionThrown = true;
    }

    return exceptionThrown;
}

int main() {
    assert(parse_movie_id("155") == 155);
    assert(parse_movie_id("42") == 42);
    assert(parse_movie_id("1") == 1);
    assert(throws_on_invalid_id("155abc"));
    assert(throws_on_invalid_id("-1"));
    assert(throws_on_invalid_id("abc"));
    assert(throws_on_invalid_id("0"));

    return 0;
}