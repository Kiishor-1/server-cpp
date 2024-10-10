#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"
#include "models/review.h"
#include "read_reviews_controller.h"

// Function to handle the request for reading reviews
void handleReadReviews(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& listingId) {
    // Create a MongoDB client connection
    auto client = mongoClient.createClient();
    auto listingCollection = client["wanderlust2"]["listings"];
    auto reviewCollection = client["wanderlust2"]["reviews"];

    // Validate collections
    if (!listingCollection || !reviewCollection) {
        res.code = 500;
        res.write("No valid collections");
        res.end();
        return;
    }

    try {
        // Fetch the listing to get its reviews
        auto listing = listingCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{listingId} << bsoncxx::builder::stream::finalize);

        // Check if the listing exists
        if (!listing) {
            res.code = 404;
            res.write("Listing not found");
            res.end();
            return;
        }

        // Get the reviews array from the listing document
        auto reviewsArray = listing->view()["reviews"].get_array().value;

        // Prepare the response JSON object
        crow::json::wvalue response;
        std::vector<crow::json::wvalue> reviews_list; // Vector to store the reviews

        // Fetch each review by its ID and add to the reviews list
        for (const auto& reviewId : reviewsArray) {
            auto review = reviewCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << reviewId.get_oid().value << bsoncxx::builder::stream::finalize);
            
            if (review) {
                // Append the review to the reviews list after converting BSON to JSON
                reviews_list.push_back(crow::json::load(bsoncxx::to_json(review->view())));
            }
        }

        // Add the reviews list to the response object
        response["reviews"] = std::move(reviews_list);

        // Send a success response with the reviews
        res.code = 200;
        res.write(crow::json::dump(response));
    }
    catch (const mongocxx::exception& e) {
        // Catch MongoDB-specific exceptions
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to read reviews: ");
        res.write(e.what());
    }
    catch (const std::exception& e) {
        // Catch standard exceptions
        std::cerr << "Standard exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to read reviews: ");
        res.write(e.what());
    }
    catch (...) {
        // Catch all other unknown exceptions
        std::cerr << "Unknown exception" << std::endl;
        res.code = 500;
        res.write("Failed to read reviews: Unknown error");
    }

    // End the response
    res.end();
}
