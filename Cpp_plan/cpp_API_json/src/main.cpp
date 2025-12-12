#include "YTSClient.h"
#include <iostream>
#include <cstdlib> // atoi

int main(int argc, char* argv[]) {
    YTSClient client;

    int choice = 0;
    int movie_id = 0;

    if (argc == 3) {
        // Use CLI arguments
        choice = std::atoi(argv[1]);
        movie_id = std::atoi(argv[2]);
    } else {
        // Interactive mode
        std::cout << "Choose option:\n"
                  << "1) List top movies\n"
                  << "2) Movie details by ID (full JSON)\n"
                  << "3) Movie details pretty (title, year, rating, torrents)\n"
                  << "Choice: ";
        std::cin >> choice;
    }

    if (choice == 1) {
        auto list = client.getMovieList("https://yts.lt/api/v2/list_movies.json?sort=seeds&limit=10");
        std::cout << "Top Movies:\n";
        for (auto& m : list) {
            std::cout << m.id << " | " << m.title << " | " << m.year << " | Rating: " << m.rating << "\n";
        }

        if (argc != 3) { // Only ask movie_id in interactive mode
            std::cout << "Enter Movie ID to see details: ";
            std::cin >> movie_id;
            auto details = client.getMovieDetailsPretty(movie_id);
            for (auto& m : details) {
                std::cout << "Title: " << m.title << "\n";
                std::cout << "Year: " << m.year << "\n";
                std::cout << "Rating: " << m.rating << "\n";
                std::cout << "Torrents:\n";
                for (auto& t : m.torrents) {
                    std::cout << "  Hash: " << t.hash << " | Quality: " << t.quality << "\n";
                }
            }
        }

    } else if (choice == 2) {
        if (argc != 3) {
            std::cout << "Enter Movie ID: ";
            std::cin >> movie_id;
        }
        auto details = client.getMovieDetails(movie_id);
        std::cout << details.dump(4) << "\n";

    } else if (choice == 3) {
        if (argc != 3) {
            std::cout << "Enter Movie ID: ";
            std::cin >> movie_id;
        }
        auto movies = client.getMovieDetailsPretty(movie_id);
        for (auto& m : movies) {
            std::cout << "Title: " << m.title << "\n";
            std::cout << "Year: " << m.year << "\n";
            std::cout << "Rating: " << m.rating << "\n";
            std::cout << "Torrents:\n";
            for (auto& t : m.torrents) {
                std::cout << "  Hash: " << t.hash << " | Quality: " << t.quality << "\n";
            }
        }

    } else {
        std::cout << "Invalid choice\n";
    }

    return 0;
}
