//
// Created by макар on 27.10.2026.
//

#include <iostream>
#include <movie.h>

int main() {
    assert(string_format_number(1000) == "1,000");
    assert(string_format_number(185000000) == "185,000,000");
    assert(string_format_number(0) == "0");
    assert(string_format_number(42) == "42");

    return 0;
}
