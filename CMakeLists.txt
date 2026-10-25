cmake_minimum_required(VERSION 4.3)
project(MovieReach)

set(CMAKE_CXX_STANDARD 20)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(MovieReach
        main.cpp
        movie.h
        movie.cpp
        http.h
        http.cpp
        cli.cpp
        cli.h
        cli.cpp
        cli.h)

add_executable(test_movie_id
        tests/test_movie_id.cpp
        cli.cpp
)

target_include_directories(test_movie_id PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MovieReach PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
)