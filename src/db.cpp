#include "db.hpp"
#include <iostream>

Database::Database() {
    conn = mysql_init(nullptr);
}

Database::~Database() {
    if (conn) mysql_close(conn);
}

bool Database::connect(const std::string& host,
                       const std::string& user,
                       const std::string& pass,
                       const std::string& db) {
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), pass.c_str(), db.c_str(), 3306, NULL, 0)) {
        std::cerr << "Database connection failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::cout << "✅ Connected to database." << std::endl;
    return true;
}

bool Database::insertOrder(const std::string& user, const std::string& item, int qty) {
    // WARNING: naive string concatenation (example only). Use prepared statements in production.
    std::string query =
        "INSERT INTO orders (`user`, item, quantity) VALUES ('" +
        user + "','" + item + "'," + std::to_string(qty) + ")";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "❌ Insert failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::cout << "✅ Order inserted for " << user << std::endl;
    return true;
}
