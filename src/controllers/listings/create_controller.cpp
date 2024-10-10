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
#include "models/listing.h"  // Include Listing model
#include "create_controller.h"

void handleCreateListing(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &user_id)
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
        // Parse the JSON body of the request
        auto json_body = crow::json::load(req.body);

        if (!json_body)
        {
            res.code = 400;
            res.write("Invalid JSON");
            res.end();
            return;
        }

        // Check if user_id is provided
        if (user_id.empty())
        {
            res.code = 401;  // Unauthorized
            res.write("Unauthorized: Missing user_id in request");
            res.end();
            return;
        }

        // Create a `Listing` object from the JSON body
        Listing listing = Listing::fromJson(json_body);

        // Validate the listing fields
        listing.validate();

        // Convert the `Listing` object to BSON
        bsoncxx::builder::stream::document document{};
        
        try {
            // Validate that user_id is a valid ObjectId and store it as owner
            bsoncxx::oid owner_oid(user_id);  // This will throw an exception if user_id is not a valid ObjectId
            document << "owner" << owner_oid;  // Add user_id as ObjectId to the owner field
        } catch (const bsoncxx::exception& e) {
            // If user_id is not a valid ObjectId, return an error
            std::cerr << "Invalid user_id format for ObjectId: " << e.what() << std::endl;
            res.code = 400;
            res.write("Invalid user_id format. It should be a valid ObjectId.");
            res.end();
            return;
        }
        
        document << bsoncxx::builder::stream::concatenate(listing.toBson());

        // Insert the document into the collection
        collection.insert_one(document.view());

        // Send a successful response
        res.code = 200;
        res.write("Listing created successfully");
        res.end();
    }
    catch (const mongocxx::exception &e)
    {
        std::cerr << "MongoDB error: " << e.what() << std::endl;
        res.code = 500;
        res.write("Database error");
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Internal server error");
        res.end();
    }
}


