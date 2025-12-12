cmake_minimum_required(VERSION 3.16)

project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find CURL package
find_package(CURL REQUIRED)

add_executable(hello
    main.cpp
    hello.h
    hello.cpp
    third_party/json.hpp
)

# Link CURL
# Confirm you have libcurl dev on your system
# sudo apt install libcurl4-openssl-dev
target_link_libraries(hello PRIVATE CURL::libcurl)

include(GNUInstallDirs)
install(TARGETS hello
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
