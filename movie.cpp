//
// Created by макар on 27.09.2026.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <iomanip>
#include <algorithm>

#include "movie.h"
#include "http.h"

const std :: string urlcontent = "https://api.themoviedb.org/3/search/movie?api_key=";
const std :: string urlid = "https://api.themoviedb.org/3/find/";
const std :: string urlfilm = "https://api.themoviedb.org/3/movie/";
const std :: string urlpopular = "https://api.themoviedb.org/3/movie/popular?api_key=";
const std :: string urltop = "https://api.themoviedb.org/3/movie/top_rated?api_key=";

std :: string string_format_number(long long value) {
    std :: string s = std :: to_string(value);

    for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3) {
        s.insert(i, ",");
    }

    return s;
}

void print_movie(const Movie &movie) {
    std :: cout << "Title: " << movie.title << std::endl;
    std :: cout << "Release data: " << movie.release_date << std::endl;
    std :: cout << "Runtime: " << movie.runtime << " min" <<  std::endl;
    std :: cout << "Rating: " << std :: fixed << std :: setprecision(2) <<  movie.rating << std::endl;
    std :: cout << "IMDb: " << movie.imdbId << std::endl;
    std :: cout << "Genres: ";
    for (int i = 0; i < movie.genres.size(); ++i) {
        if (i == movie.genres.size() - 1) {
            std :: cout << movie.genres[i] << std::endl;
            break;
        }
        std :: cout << movie.genres[i] << ", ";
    }
    std :: cout << "Budget: $" << string_format_number(movie.budget) << std::endl;
    std :: cout << "Revenue: $" << string_format_number(movie.revenue) << std::endl;
    std :: cout << std::endl;
    std :: cout << "Overview: ";
    std :: cout << movie.overview << std::endl;
    std :: cout << "Directors: ";
    for (std :: size_t i = 0; i < movie.credits.directors.size(); ++i) {
        if (i != 0) {
            std :: cout << ", ";
        }
        std :: cout << movie.credits.directors[i];
    }
    if (movie.credits.directors.empty()) {
        std :: cout << "Unknown";
    }
    std :: cout << '\n';
    std :: cout << "Actors: ";
    const std :: size_t actorsCount = std :: min(movie.credits.actors.size(), std :: size_t{5});
    for (std :: size_t i = 0; i < actorsCount; ++i) {
        if (i != 0) {
            std :: cout << ", ";
        }
        std :: cout << movie.credits.actors[i];
    }
    if (movie.credits.actors.empty()) {
        std :: cout << "Unknown";
    }
    std :: cout << '\n';
}

int select_movie(const std ::vector <MovieSearchResult>& searchResults) {
    int shownCount = std :: min(static_cast<int>(searchResults.size()), 5);

    for (int i = 0; i < shownCount; ++i) {
        std :: cout << i + 1 << ": " << searchResults[i].title << std::endl;
    }

    std :: cout << "Select a movie:" << std::endl;

    int choice;

    if (!(std::cin >> choice)) {
        throw std::runtime_error("Invalid input.");
    }
    if (choice < 1 || choice > shownCount) {
        throw std::runtime_error("Invalid choice.");
    }
    int needId = searchResults[choice - 1].id;

    return needId;
}

Movie parse_movie(const nlohmann :: json& datamovie) {
    Movie movie;

    movie.title = datamovie["title"].get<std::string>();
    movie.release_date = datamovie["release_date"].get<std::string>();
    movie.imdbId = datamovie["imdb_id"].get<std::string>();
    movie.overview = datamovie["overview"].get<std::string>();

    movie.runtime = datamovie["runtime"].get<int>();

    movie.rating = datamovie["vote_average"].get<double>();

    movie.budget = datamovie["budget"].get<long long>();
    movie.revenue = datamovie["revenue"].get<long long>();

    for (const auto & elem : datamovie["genres"]) {
        movie.genres.push_back(elem["name"].get<std::string>());
    }

    return movie;
}

std :: vector <MovieSearchResult> parse_movie_result(const nlohmann :: json& datamovie) {
    std :: vector <MovieSearchResult> movies;

    if (datamovie["results"].empty()) {
        throw std::runtime_error("No results.");
    }

    for (const auto & elem : datamovie["results"]) {
        MovieSearchResult movieSearchResult;
        movieSearchResult.title = elem["title"].get<std::string>();
        movieSearchResult.id = elem["id"].get<int>();
        movieSearchResult.release_date = elem["release_date"].get<std::string>();
        movies.push_back(movieSearchResult);
    }

    return movies;
}

std :: vector <MovieSearchResult> get_list_movies(const std :: string &url) {
    std :: string response = http_get(url);

    nlohmann:: json datapopular = nlohmann::json::parse(response);

    return parse_movie_result(datapopular);
}

std :: vector <MovieSearchResult> search_movies(const std :: string & title) {
    std :: unique_ptr<char, decltype(&curl_free)> encodedTitle(curl_easy_escape(nullptr, title.c_str(), 0), &curl_free);

    if (encodedTitle == nullptr) {
        throw std::runtime_error("curl_easy_escape failed.");
    }

    std :: string searchUrl = urlcontent + get_api_key() + "&query=" + encodedTitle.get();

    return get_list_movies(searchUrl);
}

Movie get_movie(int needId) {
    std :: string movieUrl = urlfilm + std :: to_string(needId) + "?api_key=" + get_api_key();

    std :: string movieResponse = http_get(movieUrl);

    nlohmann :: json datamovie = nlohmann ::json::parse(movieResponse);

    Movie movie = parse_movie(datamovie);

    MovieCredits movieCredits = get_movie_credits(needId);

    movie.credits.actors = movieCredits.actors;
    movie.credits.directors = movieCredits.directors;

    return movie;
}

int get_movie_by_id(const std :: string & id) {
    std :: string idUrl = urlid + id + "?external_source=imdb_id&api_key=" + get_api_key();

    std :: string response = http_get(idUrl);

    nlohmann:: json dataid = nlohmann::json::parse(response);

    if (dataid["movie_results"].empty()) {
        throw std::runtime_error("No results.");
    }

    return dataid["movie_results"][0]["id"];
}

std :: vector <MovieSearchResult> get_popular_movies() {
    return get_list_movies(urlpopular + get_api_key());
}

std :: vector <MovieSearchResult> get_top_movies() {
    return get_list_movies(urltop + get_api_key());
}

void show_movie_from_list(const std :: vector <MovieSearchResult> &movies) {
    int needId = select_movie(movies);

    Movie movie = get_movie(needId);

    print_movie(movie);
}

std :: string join_arguments(int argc, char *argv[], int startIndex) {
    std :: string finaltitle;

    for (int i = startIndex; i < argc; i++) {
        if (i > startIndex) {
            finaltitle += ' ';
        }
        finaltitle += argv[i];
    }

    return finaltitle;
}

void print_help() {
    std :: cout << "MovieReach — Movie Information CLI\n\n"
        "Usage: MovieReach <command> [arguments]\n\n"
        "Commands:\n"
        "  search <title>    Search movies by title\n"
        "  imdb <imdb_id>    Get movie by IMDb ID\n"
        "  id <tmdb_id>      Get movie by TMDB ID\n"
        "  popular           Show popular movies\n"
        "  top               Show top-rated movies\n"
        "  help              Show this help message\n"
    ;
}

MovieCredits parse_movie_credits(const nlohmann :: json& data) {
    MovieCredits movieCredits;
    for (const auto & elem : data["cast"]) {
        movieCredits.actors.push_back(elem["name"].get<std::string>());
    }

    for (const auto & elem : data["crew"]) {
        if (elem["job"] == "Director") {
            movieCredits.directors.push_back(elem["name"].get<std::string>());
        }
    }

    return movieCredits;
}

MovieCredits get_movie_credits(int movieId) {
    std :: string moviecreditsUrl = urlfilm + std :: to_string(movieId) + "/credits?api_key=" + get_api_key();

    std :: string response = http_get(moviecreditsUrl);

    return parse_movie_credits(nlohmann::json::parse(response));
}