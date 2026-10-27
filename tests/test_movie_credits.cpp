//
// Created by макар on 27.10.2026.
//

#include <movie.h>
#include <nlohmann/json.hpp>
#include <cassert>

const nlohmann::json data = {
    {"cast", {
            {{"name", "Actor A"}},
            {{"name", "Actor B"}}
    }},
    {"crew", {
            {{"name", "Director A"}, {"job", "Director"}},
            {{"name", "Editor A"}, {"job", "Editor"}},
            {{"name", "Director B"}, {"job", "Director"}}
    }}
};

const nlohmann::json emptyData = {
    {"cast", nlohmann::json::array()},
    {"crew", nlohmann::json::array()}
};

int main() {
    const MovieCredits credits = parse_movie_credits(data);

    assert(credits.actors.size() == 2);
    assert(credits.actors[0] == "Actor A");
    assert(credits.actors[1] == "Actor B");
    assert(credits.directors.size() == 2);
    assert(credits.directors[0] == "Director A");
    assert(credits.directors[1] == "Director B");

    const MovieCredits credits2 = parse_movie_credits(emptyData);

    assert(credits2.actors.empty());
    assert(credits2.directors.empty());

    return 0;
}