//
// Created by макар on 25.10.2026.
//

#include <cassert>
#include "cli.h"

int main() {
    assert(parse_movie_id("155") == 155);
    assert(parse_movie_id("42") == 42);
    assert(parse_movie_id("1") == 1);

    bool exceptionThrown = false;

    try {
        parse_movie_id("155abc");
    }
    catch (std::exception & e) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    return 0;
}