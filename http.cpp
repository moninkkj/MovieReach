//
// Created by макар on 27.09.2026.
//
#include <curl/curl.h>
#include "http.h"

#include <iostream>
#include <ostream>
#include <memory>

std :: string get_api_key() {
    const char* key = std :: getenv("API_KEY");

    if (key == nullptr) {
        throw std::runtime_error("API_KEY not set.");
    }

    return key;
}

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* response = static_cast<std::string*>(userdata);
    response->append(ptr, size * nmemb);
    return size * nmemb;
}

std:: string http_get(const std::string& url) {
    std :: string response;
    std :: unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), &curl_easy_cleanup);

    if (curl == nullptr) {
        throw std::runtime_error("curl_easy_init() failed.");
    }

    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    CURLcode result = curl_easy_perform(curl.get());
    if (result != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(result));
    }

    long status;
    curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &status);

    if (status != 200) {
        throw std::runtime_error("HTTP error: " + std::to_string(status));
    }

    return response;
}
