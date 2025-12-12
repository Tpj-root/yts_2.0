#include "Database.h"
#include "MovieRepository.h"
#include <iostream>

int main() {
    Database db("data/movies.db");
    MovieRepository repo(db);

    repo.createTables();

    repo.insertMovie({71658, "The Toxic Avenger"});
    repo.insertMovie({10010, "Avatar"});

    auto movies = repo.getAllMovies();

    std::cout << "Movies:\n";
    for (auto& m : movies) {
        std::cout << m.movie_id << " - " << m.title << "\n";
    }
}
