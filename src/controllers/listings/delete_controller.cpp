#include "delete_controller.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>

void handleDeleteListing(const crow::request& req, crow::response& res, const std::string& id, mongocxx::collection& collection) {
    try {
        auto result = collection.delete_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{id} << bsoncxx::builder::stream::finalize);
        if (result && result->deleted_count()) {
            res.code = 200;
            res.write("Listing deleted");
        } else {
            res.code = 404;
            res.write("Listing not found");
        }
    } catch (const std::exception& e) {
        res.code = 500;
        res.write("Failed to delete listing: ");
        res.write(e.what());
    }
    res.end();
}


