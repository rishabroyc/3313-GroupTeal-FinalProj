// main.cpp — Crow C++ backend server
#include "crow.h"
#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>
#include <random>
#include <cpr/cpr.h>

using namespace std;

string API_KEY = "3f460dee48e24913b07ab8659250bc53";

// ---------------------------
// Data Structures
// ---------------------------
struct User {
    string username;
    string password;
    double balance = 10000.0;
    unordered_map<string, int> portfolio; // stock symbol -> shares
};

unordered_map<string, User> users;
vector<string> tickers = {"AAPL", "MSFT", "GOOGL", "TSLA", "AMZN"};
unordered_map<string, double> stockPrices;


mutex usersMutex;
mutex pricesMutex;

void updateStockPrices() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(60));

        for (const string& symbol : tickers) {
            string url = "https://api.twelvedata.com/price?symbol=" + symbol + "&apikey=" + API_KEY;

            try {
                auto r = cpr::Get(cpr::Url{url});
                if (r.status_code == 200) {
                    auto json = crow::json::load(r.text);
                    if (json && json["price"].t() == crow::json::type::String) {
                        double price = stod(string(json["price"]));
                        lock_guard<mutex> lock(pricesMutex);
                        stockPrices[symbol] = price;
                    }
                }
            } catch (...) {
                // Fails silently — can log later
            }
        }
    }
}

// ---------------------------
// Helper: Get JSON Portfolio
// ---------------------------
crow::json::wvalue getUserPortfolio(const User& user) {
    crow::json::wvalue result;
    result["balance"] = user.balance;

    crow::json::wvalue holdings;
    for (auto& [symbol, shares] : user.portfolio) {
        holdings[symbol] = shares;
    }
    result["portfolio"] = std::move(holdings);
    return result;
}

// ---------------------------
// Main Server
// ---------------------------
int main() {
    crow::SimpleApp app;

    // Launch stock price simulation thread
    thread(updateStockPrices).detach();

    // -----------------------
    // Register
    // -----------------------
    CROW_ROUTE(app, "/register").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        string username = body["username"].s();
        string password = body["password"].s();

        lock_guard<mutex> lock(usersMutex);
        if (users.count(username)) return crow::response(409, "User exists");

        users[username] = User{username, password};
        return crow::response(200, "Registered");
    });

    // -----------------------
    // Login
    // -----------------------
    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        string username = body["username"].s();
        string password = body["password"].s();

        lock_guard<mutex> lock(usersMutex);
        if (!users.count(username) || users[username].password != password)
            return crow::response(401, "Invalid credentials");

        return crow::response(200, "Login successful");
    });

    // -----------------------
    // Get Portfolio
    // -----------------------
    CROW_ROUTE(app, "/portfolio/<string>")([](const crow::request&, string username){
        lock_guard<mutex> lock(usersMutex);
        if (!users.count(username)) return crow::response(404);
        return crow::response(getUserPortfolio(users[username]));
    });

    // -----------------------
    // Get Stock Prices
    // -----------------------
    CROW_ROUTE(app, "/stocks")([](){
        crow::json::wvalue json;
        lock_guard<mutex> lock(pricesMutex);
        for (auto& [symbol, price] : stockPrices) {
            json[symbol] = price;
        }
        return json;
    });

    // -----------------------
    // Trade (buy/sell)
    // -----------------------
    CROW_ROUTE(app, "/trade").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        string username = body["username"].s();
        string symbol = body["symbol"].s();
        string type = body["type"].s(); // "buy" or "sell"
        int quantity = body["quantity"].i();

        lock_guard<mutex> userLock(usersMutex);
        lock_guard<mutex> priceLock(pricesMutex);

        if (!users.count(username)) return crow::response(404);
        if (!stockPrices.count(symbol)) return crow::response(404);

        User& user = users[username];
        double price = stockPrices[symbol];
        double total = price * quantity;

        if (type == "buy") {
            if (user.balance < total) return crow::response(400, "Insufficient funds");
            user.balance -= total;
            user.portfolio[symbol] += quantity;
        } else if (type == "sell") {
            if (user.portfolio[symbol] < quantity) return crow::response(400, "Not enough shares");
            user.balance += total;
            user.portfolio[symbol] -= quantity;
        } else {
            return crow::response(400, "Invalid trade type");
        }

        return crow::response(200, "Trade successful");
    });

    app.port(18080).multithreaded().run();
}
