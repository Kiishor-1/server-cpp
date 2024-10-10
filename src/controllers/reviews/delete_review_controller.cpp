#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <crow/app.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"
#include "middlewares/auth_middleware.h"
#include "delete_review_controller.h"

void handleDeleteReview(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &reviewId, const std::string &listingId, const std::string &userId)
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
        // Fetch the review by its ID
        auto review = reviewCollection.find_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{reviewId} << bsoncxx::builder::stream::finalize);
        
        if (!review)
        {
            res.code = 404;
            res.write("Review not found");
            res.end();
            return;
        }

        // Ensure the review belongs to the user making the request
        std::string reviewUserId = review->view()["user"].get_oid().value.to_string();
        if (reviewUserId != userId)
        {
            res.code = 403;
            res.write("Unauthorized to delete this review");
            res.end();
            return;
        }

        // Delete the review from the reviews collection
        auto deleteResult = reviewCollection.delete_one(bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{reviewId} << bsoncxx::builder::stream::finalize);
        if (!deleteResult || deleteResult->deleted_count() == 0)
        {
            res.code = 500;
            res.write("Failed to delete review");
            res.end();
            return;
        }

        // Remove the review ID from the listing's reviews array using the listingId provided
        auto updateResult = listingCollection.update_one(
            bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{listingId} << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{} << "$pull" << bsoncxx::builder::stream::document{} << "reviews" << bsoncxx::oid{reviewId} << bsoncxx::builder::stream::finalize
        );

        if (!updateResult || updateResult->modified_count() == 0)
        {
            res.code = 500;
            res.write("Failed to update listing after review deletion");
            res.end();
            return;
        }

        res.code = 200;
        res.write("Review deleted successfully");
        res.end();
    }
    catch (const mongocxx::exception &e)
    {
        res.code = 500;
        res.write("Database error: ");
        res.write(e.what());
        res.end();
    }
    catch (const std::exception &e)
    {
        res.code = 500;
        res.write("Internal server error: ");
        res.write(e.what());
        res.end();
    }
}

