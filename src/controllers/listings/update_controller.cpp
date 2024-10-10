#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <crow/app.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/exception/exception.hpp>
#include "database/mongo_client.h"
#include "middlewares/auth_middleware.h"
#include "models/listing.h" // Include the Listing model

void handleUpdateListing(const crow::request &req, crow::response &res, const std::string &id, MongoClient &mongoClient, const std::string &user_id)
{
    auto client = mongoClient.createClient();
    auto collection = client["wanderlust2"]["listings"];

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

        // Retrieve the existing listing from the database
        auto listing_doc = collection.find_one(filter.view());
        if (!listing_doc)
        {
            res.code = 404;
            res.write("Listing not found");
            res.end();
            return;
        }

        auto listing_view = listing_doc->view();
        std::string owner = listing_view["owner"].get_oid().value.to_string();
        // Check if the user is the owner of the listing
        if (owner != user_id)
        {
            res.code = 403; // Forbidden
            res.write("Unauthorized: You are not the owner of this listing");
            res.end();
            return;
        }
        std::string category = json_body["category"].s();
        std::cout << "category is " << category << std::endl;

        // Create a Listing object from the current database document
        Listing current_listing = Listing::fromJson(crow::json::load(bsoncxx::to_json(listing_view)));

        // Update fields provided in the request body
        if (json_body.has("country"))
            current_listing.country = json_body["country"].s();
        if (json_body.has("category"))
            current_listing.category = json_body["country"].s();
        if (json_body.has("location"))
            current_listing.location = json_body["location"].s();
        if (json_body.has("price"))
            current_listing.price = json_body["price"].d();
        if (json_body.has("priceAfterTax"))
            current_listing.priceAfterTax = json_body["priceAfterTax"].d();
        if (json_body.has("title"))
            current_listing.title = json_body["title"].s();
        if (json_body.has("description"))
            current_listing.description = json_body["description"].s();


        if (json_body.has("category"))
        {
            std::string category = json_body["category"].s();
            // Validate category using the static method
            if (!Listing::isValidCategory(category))
            {
                res.code = 400;
                res.write("Invalid category value: " + category);
                res.end();
                return;
            }
            current_listing.category = category;
        }
        if (json_body.has("image"))
        {
            current_listing.image.url = json_body["image"]["url"].s();
            current_listing.image.filename = json_body["image"]["filename"].s();
        }
        if (json_body.has("geometry"))
        {
            current_listing.geometry.coordinates[0] = json_body["geometry"]["coordinates"][0].d();
            current_listing.geometry.coordinates[1] = json_body["geometry"]["coordinates"][1].d();
        }


        // Validate updated listing
        current_listing.validate();

        // Convert the updated listing back to BSON
        bsoncxx::builder::stream::document update_doc{};
        update_doc << "$set" << bsoncxx::builder::stream::open_document
                   << bsoncxx::builder::stream::concatenate(current_listing.toBson())
                   << bsoncxx::builder::stream::close_document;

        auto result = collection.update_one(filter.view(), update_doc.view());
        if (result && result->modified_count() == 1)
        {
            auto updated_listing = collection.find_one(filter.view());
            if (updated_listing)
            {
                res.code = 200;
                res.write(bsoncxx::to_json(updated_listing->view()));
            }
            else
            {
                res.code = 404;
                res.write("Listing not found");
            }
        }
        else
        {
            res.code = 404;
            res.write("Listing not found");
        }
        res.end();
    }
    catch (const mongocxx::exception &e)
    {
        res.code = 500;
        res.write("Failed to update listing: ");
        res.write(e.what());
        res.end();
    }
    catch (const std::exception &e)
    {
        res.code = 500;
        res.write("Failed to update listing: ");
        res.write(e.what());
        res.end();
    }
}