#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <crow/app.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"
#include "middlewares/auth_middleware.h"
#include "delete_controller.h"

void handleDeleteListing(const crow::request &req, crow::response &res, const std::string &id, MongoClient &mongoClient, const std::string &user_id)
{
    auto client = mongoClient.createClient();
    auto collection = client["wanderlust2"]["listings"];
    auto reviewsCollection = client["wanderlust2"]["reviews"]; // Reviews collection

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
        bsoncxx::oid oid{id};
        bsoncxx::builder::stream::document filter{};
        filter << "_id" << oid;

        // Retrieve the listing from the database to check ownership and get reviews
        auto listing = collection.find_one(filter.view());
        if (!listing)
        {
            res.code = 404;
            res.write("Listing not found");
            res.end();
            return;
        }
        auto listing_view = listing->view();
        std::string owner = listing_view["owner"].get_oid().value.to_string();

        // Check if the user is the owner of the listing
        if (owner != user_id)
        {
            res.code = 403; // Forbidden
            res.write("Unauthorized: You are not the owner of this listing");
            res.end();
            return;
        }

        // Log the reviews field to see its structure
        if (listing_view["reviews"] && listing_view["reviews"].type() == bsoncxx::type::k_array)
        {
            auto reviews_array = listing_view["reviews"].get_array().value;
            std::cout << "Reviews field found: " << bsoncxx::to_json(reviews_array) << std::endl;

            // Iterate over the review IDs and delete each corresponding review
            for (const auto &review_element : reviews_array)
            {
                std::cout << "Processing review element of type: " << bsoncxx::to_string(review_element.type()) << std::endl;

                // Only proceed if the element is of type OID
                if (review_element.type() == bsoncxx::type::k_oid)
                {
                    bsoncxx::oid review_oid = review_element.get_oid().value;

                    bsoncxx::builder::stream::document review_filter{};
                    review_filter << "_id" << review_oid;

                    // Delete the review
                    auto delete_result = reviewsCollection.delete_one(review_filter.view());
                    if (!delete_result || delete_result->deleted_count() == 0)
                    {
                        std::cerr << "Failed to delete review with ID: " << review_oid.to_string() << std::endl;
                    }
                    else
                    {
                        std::cout << "Deleted review with ID: " << review_oid.to_string() << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Skipping non-OID review element of type: " << bsoncxx::to_string(review_element.type()) << std::endl;
                }
            }
        }
        else
        {
            std::cout << "No reviews field or reviews is not an array" << std::endl;
        }

        // Proceed to delete the listing
        auto result = collection.delete_one(filter.view());
        if (result && result->deleted_count() == 1)
        {
            res.code = 200;
            res.write("Listing and associated reviews deleted successfully");
        }
        else
        {
            res.code = 404;
            res.write("Listing not found or could not be deleted");
        }
        res.end();
    }
    catch (const mongocxx::exception &e)
    {
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to delete listing: ");
        res.write(e.what());
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cerr << "General exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to delete listing: ");
        res.write(e.what());
        res.end();
    }
}


