#include "user_profile.h"
#include "crow.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/builder/stream/document.hpp> // Required for stream builder
#include <bsoncxx/builder/stream/helpers.hpp>  // Required for finalize
#include <mongocxx/collection.hpp>
#include <mongocxx/cursor.hpp>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <string>
#include <iostream>

void handleGetUserProfile(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &user_id) {
    auto client = mongoClient.createClient();
    auto userCollection = client["wanderlust2"]["users"];
    auto bookingsCollection = client["wanderlust2"]["bookings"];
    auto listingsCollection = client["wanderlust2"]["listings"]; // To fetch listing details

    std::cout << "UserId received for profile fetch: " << user_id << std::endl;
    
    try {
        // Build the query to find the user by their ObjectId
        bsoncxx::builder::basic::document query_builder{};
        query_builder.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid(user_id)));
        auto maybe_user = userCollection.find_one(query_builder.view());

        if (!maybe_user) {
            // If no user is found, return 404
            res.code = 404; 
            res.write("User not found");
            res.end();
            return;
        }

        // Extract the user document from the result
        auto userDoc = maybe_user->view();
        
        // Prepare the JSON response
        crow::json::wvalue userResponse;
        userResponse["username"] = std::string(userDoc["username"].get_string().value); // Use get_string and cast to std::string
        userResponse["_id"] = userDoc["_id"].get_oid().value.to_string(); // No change here

        // Check for optional fields and include them if present
        if (userDoc.find("firstName") != userDoc.end()) {
            userResponse["firstName"] = std::string(userDoc["firstName"].get_string().value);
        }
        if (userDoc.find("lastName") != userDoc.end()) {
            userResponse["lastName"] = std::string(userDoc["lastName"].get_string().value);
        }
        if (userDoc.find("bio") != userDoc.end()) {
            userResponse["bio"] = std::string(userDoc["bio"].get_string().value);
        }
        if (userDoc.find("email") != userDoc.end()) {
            userResponse["email"] = std::string(userDoc["email"].get_string().value);
        }

        // Fetch all bookings related to the user from the 'bookings' collection
        bsoncxx::builder::basic::document bookings_query{};
        bookings_query.append(bsoncxx::builder::basic::kvp("user", bsoncxx::oid(user_id)));

        auto cursor = bookingsCollection.find(bookings_query.view());

        std::vector<crow::json::wvalue> bookingsArray; // Vector to store bookings

        for (auto&& doc : cursor) {
            auto bookingView = doc;

            // Fetch the listing details for the booked listing
            bsoncxx::oid listingId = bookingView["listing"].get_oid().value;
            auto listing = listingsCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << listingId << bsoncxx::builder::stream::finalize);

            if (listing) {
                // Convert the listing BSON document to JSON
                crow::json::wvalue listingDetails = crow::json::load(bsoncxx::to_json(listing->view()));

                // Create a JSON booking object and replace the 'listing' ObjectId with full listing details
                crow::json::wvalue bookingJson = crow::json::load(bsoncxx::to_json(bookingView));
                bookingJson["listing"] = std::move(listingDetails); // Use move instead of copy

                // Add the modified booking JSON to the bookings array
                bookingsArray.push_back(std::move(bookingJson)); // Push the moved bookingJson
            }
        }

        // Add the bookings list to the response object
        userResponse["bookings"] = std::move(bookingsArray);

        // Set the appropriate content type for the response
        res.set_header("Content-Type", "application/json");

        // Serialize the response and send it
        res.write(crow::json::dump(userResponse)); // Use dump instead of serialize to get a JSON string
        
        res.end();
    } catch (const mongocxx::exception &e) {
        // Handle any database exceptions
        res.code = 500;
        res.write("Error fetching user data: " + std::string(e.what()));
        res.end();
    }
}

