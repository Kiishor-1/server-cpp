#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <crow/app.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"
#include "middlewares/auth_middleware.h"
#include "models/review.h"
#include "models/listing.h"
#include "create_review_controller.h"
#include <chrono>
#include <iomanip> // For std::put_time
#include <sstream> // For std::ostringstream

void handleCreateReview(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &user_id, const std::string &listingId)
{
    auto client = mongoClient.createClient();
    auto reviewCollection = client["wanderlust2"]["reviews"];
    auto listingCollection = client["wanderlust2"]["listings"];

    if (!reviewCollection || !listingCollection)
    {
        res.code = 500;
        res.write("No valid collections");
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

        // Create a `Review` object from the JSON body, excluding user and listing
        Review review = Review::fromJson(json_body);
        review.user = user_id;      // Set the user ID from the context
        review.listing = listingId; // Set the listing ID from the URL

        // Get current time
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
        review.date = oss.str(); // Set the date as a string in the desired format

        // Validate the review fields
        review.validate();

        // Convert the `Review` object to BSON
        bsoncxx::document::value reviewDoc = review.toBson();
        // Insert the review into the reviews collection
        auto result = reviewCollection.insert_one(reviewDoc.view());
        if (!result)
        {
            res.code = 500;
            res.write("Failed to create review");
            res.end();
            return;
        }
        bsoncxx::oid reviewId = result->inserted_id().get_oid().value; // Get the inserted review ID

        // Update the listing by adding the review ID to its reviews array
        auto updateResult = listingCollection.update_one(
            bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{listingId} << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{} << "$push" << bsoncxx::builder::stream::open_document << "reviews" << bsoncxx::oid{reviewId} << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);


        if (!updateResult || updateResult->modified_count() == 0)
        {
            res.code = 404;
            res.write("Listing not found or could not be updated");
            res.end();
            return;
        }

        res.code = 201;
        res.write("Review created successfully");
        res.end();
    }
    catch (const mongocxx::exception &e)
    {
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Database error");
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Internal server error");
        res.end();
    }
}
