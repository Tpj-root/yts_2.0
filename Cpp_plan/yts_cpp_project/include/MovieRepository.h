#pragma once
#include "Database.h"
#include "Movie.h"
#include <vector>

class MovieRepository {
public:
    MovieRepository(Database& db);

    void createTables();
    void insertMovie(const Movie& movie);
    std::vector<Movie> getAllMovies();

private:
    Database& db;
};
