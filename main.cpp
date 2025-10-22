// Minimal Crow-based C++ server with a POST /order, POST /translate, and WebSocket /ws
// Requires: crow_all.h (place into include/), nlohmann/json (install via libnlohmann-json3-dev on Ubuntu)
#include "crow_all.h"
#include "db.hpp"
#include "translate.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

int main() {
    crow::SimpleApp app;
    Database db;

    // Update DB credentials as needed (or read from env vars)
    if (!db.connect("localhost", "root", "your_password", "foodapp")) {
        std::cerr << "Failed to connect to DB. Exiting.\n";
        return 1;
    }

    CROW_ROUTE(app, "/")( [](){
        return "🍽️ Food Order API Server Running!";
    });

    CROW_ROUTE(app, "/order").methods("POST"_method)
    ([&db](const crow::request& req){
        try {
            auto body = json::parse(req.body);
            std::string user = body.at("user").get<std::string>();
            std::string item = body.at("item").get<std::string>();
            int qty = body.at("quantity").get<int>();

            if (db.insertOrder(user, item, qty)) {
                return crow::response(200, "Order received for " + item);
            } else {
                return crow::response(500, "Database insert failed");
            }
        } catch (const std::exception& e) {
            return crow::response(400, std::string("Bad request: ") + e.what());
        }
    });

    CROW_ROUTE(app, "/translate").methods("POST"_method)
    ([](const crow::request& req){
        try {
            auto body = json::parse(req.body);
            std::string text = body.at("text").get<std::string>();
            std::string target = body.at("target").get<std::string>();
            std::string res = translate_text(text, target);
            crow::response r;
            r.code = 200;
            r.set_header("Content-Type", "application/json");
            r.write(res);
            return r;
        } catch (const std::exception& e) {
            return crow::response(400, std::string("Bad request: ") + e.what());
        }
    });

    CROW_ROUTE(app, "/ws")
    .websocket()
    .onopen([](crow::websocket::connection& conn){
        std::cout << "Client connected via WebSocket." << std::endl;
        conn.send_text("Connected to live order updates!");
    })
    .onmessage([](crow::websocket::connection& conn, const std::string& data, bool){
        std::cout << "Message received: " << data << std::endl;
        // Echo back (in a real app you'd broadcast to relevant clients)
        conn.send_text("Server echo: " + data);
    })
    .onclose([](crow::websocket::connection& conn, const std::string& reason){
        std::cout << "WebSocket closed: " << reason << std::endl;
    });

    app.port(8080).multithreaded().run();
    return 0;
}
