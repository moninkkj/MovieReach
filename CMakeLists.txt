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
        cli.h)

add_executable(test_movie_id
        tests/test_movie_id.cpp
        cli.cpp
)

target_include_directories(test_movie_id PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(test_format_number
        tests/test_format_number.cpp
        movie.cpp
        http.cpp
)

add_executable(test_movie_credits
        tests/test_movie_credits.cpp
        movie.cpp
        http.cpp
)

target_include_directories(test_movie_credits PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(test_movie_credits PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

target_include_directories(test_format_number PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MovieReach PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

target_link_libraries(test_format_number PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

enable_testing()

add_test(
        NAME MovieIdTests
        COMMAND test_movie_id
)

add_test(
        NAME FormatNumbersTests
        COMMAND test_format_number
)

add_test(
        NAME MovieCreditsTest
        COMMAND test_movie_credits
)