#pragma once
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

struct Movie {
    int id;
    std::string title;
    int year;
    double rating;
};



struct TorrentInfo {
    std::string hash;
    std::string quality;
};

struct MovieInfo {
    std::string title;
    int year;
    double rating;
    std::vector<TorrentInfo> torrents;
};



class YTSClient {
public:
    YTSClient() = default;

    std::vector<Movie> getMovieList(const std::string& url);
    std::vector<MovieInfo> getMovieDetailsPretty(int movie_id);
    json getMovieDetails(int movie_id);

private:
    std::string fetchURL(const std::string& url);
};
