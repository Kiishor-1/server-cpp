#include "read_one_controller.h"
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <bsoncxx/builder/stream/document.hpp> // Include for bsoncxx::builder::stream
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleReadOneListing(const crow::request &req, crow::response &res, const std::string &id, MongoClient &mongoClient)
{
    auto client = mongoClient.createClient(); // Create a new client for each request
    auto listingCollection = client["wanderlust2"]["listings"];
    auto reviewCollection = client["wanderlust2"]["reviews"];
    auto userCollection = client["wanderlust2"]["users"]; // Added users collection

    if (!listingCollection || !reviewCollection || !userCollection)
    {
        std::cerr << "No valid collections" << std::endl;
        res.code = 500;
        res.write("No valid collections");
        res.end();
        return;
    }

    try
    {
        // Fetch the listing by its ID
        auto maybe_doc = listingCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{id} << bsoncxx::builder::stream::finalize);
        if (maybe_doc)
        {
            // Convert the listing document to JSON
            crow::json::wvalue listingJson = crow::json::load(bsoncxx::to_json(maybe_doc->view()));
            // Get the reviews array from the listing document
            auto reviewsArray = maybe_doc->view()["reviews"].get_array().value;

            // Prepare a vector to store reviews
            std::vector<crow::json::wvalue> reviews_list;

            // Fetch each review by its ID from the "reviews" collection
            for (const auto &reviewIdElement : reviewsArray)
            {
                bsoncxx::oid reviewId = reviewIdElement.get_oid().value;
                auto reviewDoc = reviewCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << reviewId << bsoncxx::builder::stream::finalize);

                if (reviewDoc)
                {
                    crow::json::wvalue reviewJson = crow::json::load(bsoncxx::to_json(reviewDoc->view()));
                    // Fetch the user details using userId from the review
                    bsoncxx::oid userId = reviewDoc->view()["user"].get_oid().value;
                    auto userDoc = userCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << userId << bsoncxx::builder::stream::finalize);
                    if (userDoc)
                    {   
                        // Add user details to the review JSON
                        reviewJson["user"]["firstName"] = userDoc->view()["firstName"].get_string().value.data();
                        reviewJson["user"]["lastName"] = userDoc->view()["lastName"].get_string().value.data();
                        reviewJson["user"]["username"] = userDoc->view()["username"].get_string().value.data();
                    }
                    else
                    {
                        // If user details not found, we can return an empty user field or handle this scenario
                        reviewJson["user"] = nullptr;
                    }
                    // Move the reviewJson into the reviews_list to avoid copying
                    reviews_list.push_back(std::move(reviewJson));
                }
            }

            // Add the populated reviews list to the listing JSON
            listingJson["reviews"] = std::move(reviews_list);

            // Send the final response
            res.code = 200;
            res.write(crow::json::dump(listingJson));
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
        res.write("Failed to read listing: ");
        res.write(e.what());
    }
    catch (const std::exception &e)
    {
        res.code = 500;
        res.write("Failed to read listing: ");
        res.write(e.what());
    }

    res.end();
}
