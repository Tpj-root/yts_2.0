#pragma once
#include <sqlite3.h>
#include <string>

class Database {
public:
    Database(const std::string& path);
    ~Database();

    bool exec(const std::string& sql);
    bool query(const std::string& sql, int (*callback)(void*,int,char**,char**), void* data);

private:
    sqlite3* db = nullptr;
};
