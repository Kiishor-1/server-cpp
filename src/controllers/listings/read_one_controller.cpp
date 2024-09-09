#include "read_one_controller.h"
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <bsoncxx/builder/stream/document.hpp>  // Include for bsoncxx::builder::stream
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"


void handleReadOneListing(const crow::request& req, crow::response& res, const std::string& id, MongoClient &mongoClient) {
    auto collection = mongoClient.getCollection("wanderlust2", "listings");
    if (!collection)
    {
        std::cerr << "No valid collection" << std::endl;
        res.code = 500;
        res.write("No valid collection");
        res.end();
        return;
    }
    try {
        auto maybe_doc = collection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{id} << bsoncxx::builder::stream::finalize);
        if (maybe_doc) {
            res.code = 200;
            res.write(bsoncxx::to_json(*maybe_doc));
        } else {
            res.code = 404;
            res.write("Listing not found");
        }
    } catch (const mongocxx::exception& e) {
        res.code = 500;
        res.write("Failed to read listing: ");
        res.write(e.what());
    } catch (const std::exception& e) {
        res.code = 500;
        res.write("Failed to read listing: ");
        res.write(e.what());
    }
    res.end();
}
