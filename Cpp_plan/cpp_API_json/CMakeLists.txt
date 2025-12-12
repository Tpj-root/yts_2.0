cmake_minimum_required(VERSION 3.10)
project(yts_cpp_api)

set(CMAKE_CXX_STANDARD 17)

add_executable(yts_cpp_api
    src/main.cpp
    src/YTSClient.cpp
)

target_include_directories(yts_cpp_api PRIVATE
    include
    third_party
)

# Use curl library for HTTP requests
find_package(CURL REQUIRED)
target_link_libraries(yts_cpp_api CURL::libcurl)
