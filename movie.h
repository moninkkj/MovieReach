//
// Created by макар on 27.09.2026.
//
#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct Movie {
    std :: string title;
    std :: string release_date;
    std :: string imdbId;
    std :: string overview;

    int runtime;
    double rating;

    long long budget;
    long long revenue;

    std :: vector<std :: string> genres;
};

struct MovieSearchResult {
    int id;
    std :: string title, release_date;
};

void print_movie(const Movie &movie);
int select_movie(const std ::vector <MovieSearchResult>& searchResults);
Movie get_movie(int needId);
std :: vector <MovieSearchResult> search_movies(const std :: string & title);
int get_movie_by_id(const std :: string & id);
std :: vector <MovieSearchResult> get_popular_movies();
std :: vector <MovieSearchResult> get_top_movies();
void show_movie_from_list(const std :: vector <MovieSearchResult> &movies);
std :: string join_arguments(int argc, char *argv[], int startIndex);
void print_help();
