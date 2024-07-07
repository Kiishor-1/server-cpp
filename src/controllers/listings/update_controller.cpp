#include "update_controller.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h> // Make sure this is included properly
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleUpdateListing(const crow::request &req, crow::response &res, const std::string &id, MongoClient &mongoClient)
{
    auto collection = mongoClient.getCollection("wanderlust2", "listings");
    if (!collection)
    {
        std::cerr << "No valid collection" << std::endl;
        res.code = 500;
        res.write("No valid collection");
        res.end();
        return;
    }
    try
    {
        auto json_body = crow::json::load(req.body);

        if (!json_body)
        {
            res.code = 400;
            res.write("Invalid JSON");
            res.end();
            return;
        }

        bsoncxx::oid oid{id};
        bsoncxx::builder::stream::document filter{};
        filter << "_id" << oid;

        bsoncxx::builder::stream::document update_doc{};
        update_doc << "$set" << bsoncxx::builder::stream::open_document;
        for (const auto &item : json_body)
        {
            if (item.t() == crow::json::type::String)
            {
                update_doc << item.key() << item.s();
            }
            else if (item.t() == crow::json::type::Number)
            {
                update_doc << item.key() << item.i();
            }
        }
        update_doc << bsoncxx::builder::stream::close_document;

        auto result = collection.update_one(filter.view(), update_doc.view());
        if (result && result->modified_count() == 1)
        {
            res.code = 200;
            res.write("Listing updated");
        }
        else
        {
            res.code = 404;
            res.write("Listing not found");
        }
    }
    catch (const mongocxx::exception &e)
    {
        res.code = 500;
        res.write("Failed to update listing: ");
        res.write(e.what());
    }
    catch (const std::exception &e)
    {
        res.code = 500;
        res.write("Failed to update listing: ");
        res.write(e.what());
    }
    res.end();
}
