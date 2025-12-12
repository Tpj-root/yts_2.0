#include <iostream>
#include "hello.h"
// #include <curl/curl.h>

using namespace std;

int main()
{
    // cout << "Hello World!" << endl;
    // Hello h;
    // h.iamhello();


    HttpClient http;
    // http.iamhello();
    std::string data = http.get("https://yts.lt/api/v2/movie_details.json?movie_id=1");
    // std::cout << data << "\n";
    auto j = JsonHelper::parse(data);

    //JsonHelper::printPretty(j);
    JsonHelper::printMovieBasic(j);

    return 0;
}
