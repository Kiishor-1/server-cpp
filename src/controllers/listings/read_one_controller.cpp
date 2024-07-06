#include "read_one_controller.h"
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <bsoncxx/builder/stream/document.hpp>  // Include for bsoncxx::builder::stream
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>


void handleReadOneListing(const crow::request& req, crow::response& res, const std::string& id, mongocxx::collection& collection) {
    try {
        auto maybe_doc = collection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{id} << bsoncxx::builder::stream::finalize);
        if (maybe_doc) {
            res.code = 200;
            res.write(bsoncxx::to_json(*maybe_doc));
        } else {
            res.code = 404;
            res.write("Listing not found");
        }
    } catch (const std::exception& e) {
        res.code = 500;
        res.write("Failed to read listing: ");
        res.write(e.what());
    }
    res.end();
}
