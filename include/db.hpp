#pragma once
#include <mysql/mysql.h>
#include <string>

class Database {
public:
    Database();
    ~Database();

    bool connect(const std::string& host,
                 const std::string& user,
                 const std::string& pass,
                 const std::string& db);

    bool insertOrder(const std::string& user, const std::string& item, int qty);

private:
    MYSQL* conn;
};
