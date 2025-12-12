#ifndef HELLO_H
#define HELLO_H
#include <string>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include "third_party/json.hpp"

class Hello
{
public:
    Hello();
    void iamhello();
};



class HttpClient : public Hello{
public:
    HttpClient() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~HttpClient() {
        curl_global_cleanup();
    }

    std::string get(const std::string& url);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};



using json = nlohmann::json;

class JsonHelper : public HttpClient{
public:
    static json parse(const std::string& text);

    static void printPretty(const json& j);
    static void printMovieBasic(const json& j);

    static std::string joinGenres(const json& arr);



};












#endif // HELLO_H
