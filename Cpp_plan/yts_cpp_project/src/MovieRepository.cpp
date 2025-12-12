#include "MovieRepository.h"
#include <iostream>

MovieRepository::MovieRepository(Database& db) : db(db) {}

void MovieRepository::createTables() {
    db.exec(
        "CREATE TABLE IF NOT EXISTS movies ("
        " movie_id INTEGER PRIMARY KEY,"
        " title TEXT NOT NULL"
        ");"
    );
}

void MovieRepository::insertMovie(const Movie& movie) {
    std::string sql =
        "INSERT INTO movies (movie_id, title) VALUES (" +
        std::to_string(movie.movie_id) + ", '" + movie.title + "');";

    db.exec(sql);
}

static int movieCallback(void* data, int argc, char** argv, char** colName) {
    auto* out = reinterpret_cast<std::vector<Movie>*>(data);

    Movie m;
    m.movie_id = std::stoi(argv[0]);
    m.title = argv[1] ? argv[1] : "";

    out->push_back(m);
    return 0;
}

std::vector<Movie> MovieRepository::getAllMovies() {
    std::vector<Movie> list;
    db.query("SELECT movie_id, title FROM movies;", movieCallback, &list);
    return list;
}
