// read_controller.cpp

#include "read_controller.h"
#include <bsoncxx/json.hpp>
#include <crow/json.h>

void handleReadListings(const crow::request& req, crow::response& res, mongocxx::collection& collection) {
    try {
        auto cursor = collection.find({});
        crow::json::wvalue listings;
        std::vector<crow::json::wvalue> listings_array;

        for (auto&& doc : cursor) {
            crow::json::wvalue listing = crow::json::load(bsoncxx::to_json(doc));
            listings_array.push_back(std::move(listing));
        }

        listings["listings"] = std::move(listings_array);
        
        res.code = 200;
        res.write(crow::json::dump(listings));
    } catch (const std::exception& e) {
        res.code = 500;
        res.write("Failed to read listings: ");
        res.write(e.what());
    }
    res.end();
}




