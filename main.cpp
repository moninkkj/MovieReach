#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <stdexcept>
#include <memory>

#include "movie.h"
#include "cli.h"

int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            throw std:: out_of_range ("Incorrect number of arguments");
        }

        std :: string command = std::string(argv[1]);

        if (command == "search") {

            std :: string finaltitle;

            if (argc == 2) {
                throw std:: out_of_range ("Incorrect number of arguments");
            }

            finaltitle = join_arguments(argc, argv, 2);

            std :: cout << "Searching for: " + finaltitle << std::endl;

            const std :: vector <MovieSearchResult> searchResults = search_movies(finaltitle);

            show_movie_from_list(searchResults);

            return 0;
        }
        if (command == "imdb") {
            if (argc != 3) {
                throw std:: out_of_range ("Incorrect number of arguments");
            }

            std :: string imdbId = argv[2];

            int needId = get_movie_by_id(imdbId);

            Movie movie = get_movie(needId);

            print_movie(movie);

            return 0;
        }
        if (command == "id") {
            if (argc > 3) {
                throw std:: out_of_range ("Incorrect number of arguments");
            }
            int id = parse_movie_id(argv[2]);

            Movie movie = get_movie(id);

            print_movie(movie);

            return 0;
        }
        if (command == "popular") {
            if (argc > 2) {
                throw std:: out_of_range ("Incorrect number of arguments");
            }

            const std :: vector <MovieSearchResult> movies =  get_popular_movies();

            show_movie_from_list(movies);

            return 0;
        }
        if (command == "top") {
            if (argc > 2) {
                throw std:: out_of_range ("Incorrect number of arguments");
            }

            const std :: vector <MovieSearchResult> movies = get_top_movies();

            show_movie_from_list(movies);

            return 0;
        }

        if (command == "help") {


            if (argc > 2) {
                throw std:: out_of_range ("There must be only command");
            }

            print_help();

            return 0;
        }
    }
    catch (const std::exception & e) {
        std :: cout << e.what() << std::endl;
        return 1;
    }
    std :: cout << "Incorrect command" << std::endl;

    return 1;
}
