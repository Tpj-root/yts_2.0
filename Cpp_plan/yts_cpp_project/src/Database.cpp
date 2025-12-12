#include "Database.h"
#include "sqlite3.h"
#include <iostream>

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db)) {
        std::cerr << "DB open failed\n";
        db = nullptr;
    }
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

bool Database::exec(const std::string& sql) {
    char* err = nullptr;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &err) != SQLITE_OK) {
        std::cerr << "SQL error: " << err << "\n";
        sqlite3_free(err);
        return false;
    }
    return true;
}

bool Database::query(const std::string& sql, int (*callback)(void*,int,char**,char**), void* data) {
    char* err = nullptr;
    if (sqlite3_exec(db, sql.c_str(), callback, data, &err) != SQLITE_OK) {
        std::cerr << "Query error: " << err << "\n";
        sqlite3_free(err);
        return false;
    }
    return true;
}
