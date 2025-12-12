#include "YTSClient.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string YTSClient::fetchURL(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl failed: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::vector<Movie> YTSClient::getMovieList(const std::string& url) {
    std::vector<Movie> list;
    std::string jsonStr = fetchURL(url);
    auto j = json::parse(jsonStr, nullptr, false);

    if (j.is_discarded() || j["status"] != "ok") {
        std::cerr << "Invalid JSON or API failed\n";
        return list;
    }

    auto movies = j["data"]["movies"];
    for (auto& m : movies) {
        Movie movie;
        movie.id = m["id"].get<int>();
        movie.title = m["title"].get<std::string>();
        movie.year = m["year"].get<int>();
        movie.rating = m["rating"].get<double>();
        list.push_back(movie);
    }
    return list;
}

json YTSClient::getMovieDetails(int movie_id) {
    std::string url = "https://yts.lt/api/v2/movie_details.json?movie_id=" + std::to_string(movie_id);
    std::string jsonStr = fetchURL(url);
    auto j = json::parse(jsonStr, nullptr, false);
    if (j.is_discarded() || j["status"] != "ok") {
        std::cerr << "Failed to fetch movie details\n";
    }
    return j;
}

std::vector<MovieInfo> YTSClient::getMovieDetailsPretty(int movie_id) {
    std::vector<MovieInfo> result;

    std::string url = "https://yts.lt/api/v2/movie_details.json?movie_id=" + std::to_string(movie_id);
    std::string jsonStr = fetchURL(url);
    auto j = json::parse(jsonStr, nullptr, false);

    if (j.is_discarded() || j["status"] != "ok") {
        std::cerr << "Failed to fetch movie details\n";
        return result;
    }

    auto movieJson = j["data"]["movie"];

    MovieInfo movie;
    movie.title = movieJson["title"].get<std::string>();
    movie.year = movieJson["year"].get<int>();
    movie.rating = movieJson["rating"].get<double>();

    for (auto& t : movieJson["torrents"]) {
        TorrentInfo torrent;
        torrent.hash = t["hash"].get<std::string>();
        torrent.quality = t["quality"].get<std::string>();
        movie.torrents.push_back(torrent);
    }

    result.push_back(movie);
    return result;
}


