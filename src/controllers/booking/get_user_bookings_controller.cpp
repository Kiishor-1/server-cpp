// #include <mongocxx/collection.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <bsoncxx/json.hpp>
// #include <crow/json.h>
// #include <iostream>
// #include <mongocxx/exception/exception.hpp>
// #include "database/mongo_client.h"
// #include "middlewares/auth_middleware.h"

// // Function to handle the request for getting user bookings
// void handleGetUserBookings(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& userId) {
//     // Create a MongoDB client connection
//     auto client = mongoClient.createClient();
//     auto bookingsCollection = client["wanderlust2"]["bookings"];
//     auto usersCollection = client["wanderlust2"]["users"];

//     // Validate collections
//     if (!bookingsCollection || !usersCollection) {
//         res.code = 500;
//         res.write("No valid collections");
//         res.end();
//         return;
//     }

//     try {
//         // Fetch the user by their ID to get their bookings
//         auto user = usersCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{userId} << bsoncxx::builder::stream::finalize);

//         // Check if the user exists
//         if (!user) {
//             res.code = 404;
//             res.write("User not found");
//             res.end();
//             return;
//         }

//         // Get the bookings array from the user document
//         auto bookingsArray = user->view()["bookings"].get_array().value;

//         // Prepare the response JSON object
//         crow::json::wvalue response;
//         std::vector<crow::json::wvalue> bookings_list;  // Vector to store the bookings

//         // Fetch each booking by its ID and add to the bookings list
//         for (const auto& bookingId : bookingsArray) {
//             auto booking = bookingsCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bookingId.get_oid().value << bsoncxx::builder::stream::finalize);

//             if (booking) {
//                 // Append the booking to the bookings list after converting BSON to JSON
//                 bookings_list.push_back(crow::json::load(bsoncxx::to_json(booking->view())));
//             }
//         }

//         // Add the bookings list to the response object
//         response["bookings"] = std::move(bookings_list);

//         // Send a success response with the bookings
//         res.code = 200;
//         res.write(crow::json::dump(response));
//     }
//     catch (const mongocxx::exception& e) {
//         // Catch MongoDB-specific exceptions
//         std::cerr << "MongoDB exception: " << e.what() << std::endl;
//         res.code = 500;
//         res.write("Failed to get user bookings: ");
//         res.write(e.what());
//     }
//     catch (const std::exception& e) {
//         // Catch standard exceptions
//         std::cerr << "Standard exception: " << e.what() << std::endl;
//         res.code = 500;
//         res.write("Failed to get user bookings: ");
//         res.write(e.what());
//     }
//     catch (...) {
//         // Catch all other unknown exceptions
//         std::cerr << "Unknown exception" << std::endl;
//         res.code = 500;
//         res.write("Failed to get user bookings: Unknown error");
//     }

//     // End the response
//     res.end();
// }


// #include <mongocxx/collection.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <bsoncxx/json.hpp>
// #include <crow/json.h>
// #include <iostream>
// #include <mongocxx/exception/exception.hpp>
// #include "database/mongo_client.h"
// #include "middlewares/auth_middleware.h"

// // Function to handle the request for getting user bookings with full listing data
// void handleGetUserBookings(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& userId) {
//     // Create a MongoDB client connection
//     auto client = mongoClient.createClient();
//     auto bookingsCollection = client["wanderlust2"]["bookings"];
//     auto listingsCollection = client["wanderlust2"]["listings"];  // Add listings collection
//     auto usersCollection = client["wanderlust2"]["users"];

//     // Validate collections
//     if (!bookingsCollection || !usersCollection || !listingsCollection) {
//         res.code = 500;
//         res.write("No valid collections");
//         res.end();
//         return;
//     }

//     try {
//         // Fetch the user by their ID to get their bookings
//         auto user = usersCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{userId} << bsoncxx::builder::stream::finalize);

//         // Check if the user exists
//         if (!user) {
//             res.code = 404;
//             res.write("User not found");
//             res.end();
//             return;
//         }

//         // Get the bookings array from the user document
//         auto bookingsArray = user->view()["bookings"].get_array().value;

//         // Prepare the response JSON object
//         crow::json::wvalue response;
//         std::vector<crow::json::wvalue> bookings_list;  // Vector to store the bookings

//         // Fetch each booking by its ID and add to the bookings list
//         for (const auto& bookingId : bookingsArray) {
//             auto booking = bookingsCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bookingId.get_oid().value << bsoncxx::builder::stream::finalize);

//             if (booking) {
//                 // Convert booking BSON to JSON
//                 crow::json::wvalue bookingJson = crow::json::load(bsoncxx::to_json(booking->view()));

//                 // Fetch the listing ID from the booking document
//                 auto listingId = booking->view()["listing"].get_oid().value;

//                 // Fetch the full listing data by listing ID
//                 auto listing = listingsCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << listingId << bsoncxx::builder::stream::finalize);

//                 if (listing) {
//                     // Convert listing BSON to JSON and add it to the booking JSON
//                     bookingJson["listing"] = crow::json::load(bsoncxx::to_json(listing->view()));
//                 } else {
//                     // If the listing is not found, handle accordingly (optional)
//                     bookingJson["listing"] = "Listing not found";
//                 }

//                 // Append the booking (with full listing data) to the bookings list
//                 bookings_list.push_back(std::move(bookingJson));
//             }
//         }

//         // Add the bookings list to the response object
//         response["bookings"] = std::move(bookings_list);

//         // Send a success response with the bookings
//         res.code = 200;
//         res.write(crow::json::dump(response));
//     }
//     catch (const mongocxx::exception& e) {
//         // Catch MongoDB-specific exceptions
//         std::cerr << "MongoDB exception: " << e.what() << std::endl;
//         res.code = 500;
//         res.write("Failed to get user bookings: ");
//         res.write(e.what());
//     }
//     catch (const std::exception& e) {
//         // Catch standard exceptions
//         std::cerr << "Standard exception: " << e.what() << std::endl;
//         res.code = 500;
//         res.write("Failed to get user bookings: ");
//         res.write(e.what());
//     }
//     catch (...) {
//         // Catch all other unknown exceptions
//         std::cerr << "Unknown exception" << std::endl;
//         res.code = 500;
//         res.write("Failed to get user bookings: Unknown error");
//     }

//     // End the response
//     res.end();
// }

#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"
#include "middlewares/auth_middleware.h"

// Function to handle the request for getting user bookings with full listing details
void handleGetUserBookings(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& userId) {
    // Create a MongoDB client connection
    auto client = mongoClient.createClient();
    auto bookingsCollection = client["wanderlust2"]["bookings"];
    auto listingsCollection = client["wanderlust2"]["listings"]; // To fetch listing details

    // Validate collections
    if (!bookingsCollection || !listingsCollection) {
        res.code = 500;
        res.write("No valid collections");
        res.end();
        return;
    }

    try {
        // Build the query to find bookings for the user by their user ID
        bsoncxx::builder::basic::document bookings_query{};
        bookings_query.append(bsoncxx::builder::basic::kvp("user", bsoncxx::oid(userId)));

        auto cursor = bookingsCollection.find(bookings_query.view());

        std::vector<crow::json::wvalue> bookingsArray; // Vector to store bookings

        // Iterate through each booking document
        for (auto&& doc : cursor) {
            auto bookingView = doc;

            // Fetch the listing details for the booked listing
            bsoncxx::oid listingId = bookingView["listing"].get_oid().value;
            auto listing = listingsCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << listingId << bsoncxx::builder::stream::finalize);

            // Create a JSON booking object and include the listing details
            crow::json::wvalue bookingJson = crow::json::load(bsoncxx::to_json(bookingView));

            if (listing) {
                // Convert the listing BSON document to JSON
                crow::json::wvalue listingDetails = crow::json::load(bsoncxx::to_json(listing->view()));
                
                // Replace the 'listing' ObjectId with full listing details
                bookingJson["listing"] = std::move(listingDetails);
            }

            // Add the modified booking JSON to the bookings array
            bookingsArray.push_back(std::move(bookingJson)); // Push the moved bookingJson
        }

        // Prepare the response JSON object
        crow::json::wvalue response;
        response["bookings"] = std::move(bookingsArray);

        // Set the appropriate content type for the response
        res.set_header("Content-Type", "application/json");

        // Send a success response with the bookings
        res.code = 200;
        res.write(crow::json::dump(response)); // Serialize the response to JSON
    }
    catch (const mongocxx::exception& e) {
        // Catch MongoDB-specific exceptions
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to get user bookings: ");
        res.write(e.what());
    }
    catch (const std::exception& e) {
        // Catch standard exceptions
        std::cerr << "Standard exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to get user bookings: ");
        res.write(e.what());
    }
    catch (...) {
        // Catch all other unknown exceptions
        std::cerr << "Unknown exception" << std::endl;
        res.code = 500;
        res.write("Failed to get user bookings: Unknown error");
    }

    // End the response
    res.end();
}

