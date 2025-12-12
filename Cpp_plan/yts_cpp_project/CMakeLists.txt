cmake_minimum_required(VERSION 3.10)
project(yts_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(yts_cpp
    src/main.cpp
    src/Database.cpp
    src/MovieRepository.cpp
    third_party/sqlite3.c
)

target_include_directories(yts_cpp PRIVATE
    include
    third_party
)
