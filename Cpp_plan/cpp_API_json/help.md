Got it! You want a **C++ project** that:

* Asks the user to choose between **list of movies** or **movie details by ID**
* Fetches the JSON online
* Parses it using `nlohmann/json` (`json.hpp`)
* Prints results nicely
* Uses **classes + methods**

Here’s a clean starting C++ project.

---

# ✅ **Project Structure**

```
yts_cpp_api/
│
├── include/
│   ├── YTSClient.h
│
├── src/
│   ├── main.cpp
│   ├── YTSClient.cpp
│
├── third_party/
│   └── json.hpp       <-- nlohmann/json single header
│
├── CMakeLists.txt
```

---

# ✅ **1. CMakeLists.txt**

```cmake
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
```

---

# ✅ **2. include/YTSClient.h**

```cpp
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

class YTSClient {
public:
    YTSClient() = default;

    std::vector<Movie> getMovieList(const std::string& url);
    json getMovieDetails(int movie_id);

private:
    std::string fetchURL(const std::string& url);
};
```

---

# ✅ **3. src/YTSClient.cpp**

```cpp
#include "YTSClient.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string YTSClient::fetchURL(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl failed: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::vector<Movie> YTSClient::getMovieList(const std::string& url) {
    std::vector<Movie> list;
    std::string jsonStr = fetchURL(url);
    auto j = json::parse(jsonStr, nullptr, false);

    if (j.is_discarded() || j["status"] != "ok") {
        std::cerr << "Invalid JSON or API failed\n";
        return list;
    }

    auto movies = j["data"]["movies"];
    for (auto& m : movies) {
        Movie movie;
        movie.id = m["id"].get<int>();
        movie.title = m["title"].get<std::string>();
        movie.year = m["year"].get<int>();
        movie.rating = m["rating"].get<double>();
        list.push_back(movie);
    }
    return list;
}

json YTSClient::getMovieDetails(int movie_id) {
    std::string url = "https://yts.lt/api/v2/movie_details.json?movie_id=" + std::to_string(movie_id);
    std::string jsonStr = fetchURL(url);
    auto j = json::parse(jsonStr, nullptr, false);
    if (j.is_discarded() || j["status"] != "ok") {
        std::cerr << "Failed to fetch movie details\n";
    }
    return j;
}
```

---

# ✅ **4. src/main.cpp**

```cpp
#include "YTSClient.h"
#include <iostream>

int main() {
    YTSClient client;

    std::cout << "Choose option:\n1) List top movies\n2) Movie details by ID\nChoice: ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        auto list = client.getMovieList("https://yts.lt/api/v2/list_movies.json?sort=seeds&limit=10");
        std::cout << "Top Movies:\n";
        for (auto& m : list) {
            std::cout << m.id << " | " << m.title << " | " << m.year << " | Rating: " << m.rating << "\n";
        }
        std::cout << "Enter Movie ID to see details: ";
        int movie_id;
        std::cin >> movie_id;
        auto details = client.getMovieDetails(movie_id);
        std::cout << details.dump(4) << "\n"; // nicely formatted JSON
    } else if (choice == 2) {
        std::cout << "Enter Movie ID: ";
        int movie_id;
        std::cin >> movie_id;
        auto details = client.getMovieDetails(movie_id);
        std::cout << details.dump(4) << "\n";
    } else {
        std::cout << "Invalid choice\n";
    }

    return 0;
}
```

---

# ✅ **5. Notes**

* Uses `nlohmann/json` (`json.hpp`) — single header, very convenient
* Uses `libcurl` for HTTP requests
* Can **list movies** and then **fetch details by ID**
* Outputs JSON nicely formatted

---

# 🔹 **Compile**

```bash
mkdir build
cd build
cmake ..
make
./yts_cpp_api
```

---

This is the **fully class-based, interactive C++ project** for fetching YTS movie lists and details.

If you want, I can **extend it** to:

* Store movies locally in **SQLite database**
* Parse genres and torrents
* CLI search by keyword

Do you want me to do that next?





Yes — if you don’t want to install **libcurl system-wide**, you can **bundle it inside your project** (in a `third_party/` folder) and build it locally.

This avoids `find_package(CURL)` errors completely.

---

# ✅ **Steps to include libcurl locally**

1. **Download curl source**:

```bash
wget https://curl.se/download/curl-8.3.0.tar.gz
tar xvf curl-8.3.0.tar.gz
mv curl-8.3.0 yts_cpp_api/third_party/curl
```

2. **Build curl locally** (inside `third_party/curl`):

```bash
cd yts_cpp_api/third_party/curl
./configure --disable-shared --enable-static --prefix=$(pwd)/build
make
make install
```

* This builds **static libcurl** in `third_party/curl/build/lib`
* Headers are in `third_party/curl/build/include`

3. **Update CMakeLists.txt**:

```cmake
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
    third_party/curl/build/include
)

target_link_directories(yts_cpp_api PRIVATE
    third_party/curl/build/lib
)

target_link_libraries(yts_cpp_api
    ssl
    crypto
    z
    pthread
    curl
)
```

4. **Build project**:

```bash
mkdir build
cd build
cmake ..
make
./yts_cpp_api
```

---

# ✅ **Alternative simpler approach (Linux)**

If you **don’t mind installing curl dev package once**, just run:

```bash
sudo apt install libcurl4-openssl-dev
```

Then `find_package(CURL REQUIRED)` works immediately.

---

# ⚡ Summary

* **Third-party libcurl** is possible (static build + include headers)
* **Recommended for full portability** (Windows/Linux cross-platform)
* If Linux only, installing via apt is easier.

---

I can prepare a **ready-to-build `third_party/` folder** with `curl + json.hpp` included, so you don’t need system packages at all.

Do you want me to do that?


