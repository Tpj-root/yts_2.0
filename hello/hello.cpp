#include "hello.h"
#include <iostream>

Hello::Hello() {
    // std::cout << "Iam cnstarctor" ;
}

void Hello::iamhello()
{
    std::cout << "Iamhellooo" ;

}

std::string HttpClient::get(const std::string &url) {
    CURL* curl = curl_easy_init();
    std::string buffer;

    if (!curl) return buffer;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        buffer = "";
    }

    curl_easy_cleanup(curl);
    return buffer;
}

size_t HttpClient::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json JsonHelper::parse(const std::string &text) {
    json j = json::parse(text, nullptr, false);

    if (j.is_discarded()) {
        std::cerr << "JSON parse error\n";
        return json{};
    }
    return j;
}

void JsonHelper::printPretty(const json &j) {
    std::cout << j.dump(4) << "\n";
}


void JsonHelper::printMovieBasic(const json &j)
    {
    if (!j.contains("data") || !j["data"].contains("movie")) {
        std::cerr << "Invalid movie JSON\n";
        return;
    }

    const json& m = j["data"]["movie"];

    std::cout << "ID      : "     << m.value("id", 0) << "\n";
    std::cout << "Title   : "  << m.value("title", "N/A") << "\n";
    std::cout << "Year    : "   << m.value("year", 0) << "\n";
    std::cout << "Rating  : " << m.value("rating", 0.0) << "\n";
    std::cout << "Runtime : "<< m.value("runtime", 0) << " min\n";
    std::cout << "IMDB    : "   << m.value("imdb_code", "N/A") << "\n";
    // std::cout << "genres    : "   << m.value("genres", "N/A") << "\n";
    std::cout << "genres  : " << JsonHelper::joinGenres(m["genres"]) << "\n";
    std::cout << "URL     : "    << m.value("url", "N/A") << "\n";
}

    std::string JsonHelper::joinGenres(const json &arr) {
        if (!arr.is_array()) return "N/A";

        std::string out;
        for (size_t i = 0; i < arr.size(); i++) {
            out += arr[i].get<std::string>();
            if (i + 1 < arr.size()) out += ", ";
        }
        return out;
    }
