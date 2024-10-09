#include "reviews.h"
#include "../controllers/reviews/create_review_controller.h"
#include "../controllers/reviews/read_reviews_controller.h"
#include "../controllers/reviews/delete_review_controller.h"
#include "../../include/middleware.h"
#include "dotenv.h"
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"

#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)

template <typename AppType>
void defineReviewsRoutes(AppType &app, MongoClient &mongoClient)
{
    auto client = mongoClient.createClient(); // Create a new client for each request
    auto collection = client["wanderlust2"]["reviews"];
    if (!collection)
    {
        throw std::runtime_error("Failed to get collection in defineReviewsRoutes");
    }

    // Create a new review
    CROW_ROUTE(app, "/listings/<string>/reviews").methods(crow::HTTPMethod::Post)([&app, &mongoClient](const crow::request &req, crow::response &res, const std::string &listingId)
                                                                                 {
        auto& auth_context = app.template get_context<AuthMiddleware>(req);  // Get AuthMiddleware context
        std::string user_id = auth_context.user_id;
        std::cout<<"check"<<std::endl;
        std::cout<<"user's ID is"<<user_id<<std::endl;
        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }

        handleCreateReview(req, res, mongoClient, user_id, listingId); });

    // Get all reviews for a listing
    CROW_ROUTE(app, "/listings/<string>/reviews").methods(crow::HTTPMethod::Get)([&mongoClient](const crow::request &req, crow::response &res, const std::string &listingId)
                                                                                 { handleReadReviews(req, res, mongoClient, listingId); });

    // Delete a review by its ID
    CROW_ROUTE(app, "/listings/<string>/reviews/<string>").methods(crow::HTTPMethod::Delete)([&app, &mongoClient](const crow::request &req, crow::response &res, const std::string &listingId, const std::string &reviewId)
                                                                                             {
    auto& auth_context = app.template get_context<AuthMiddleware>(req);  // Get AuthMiddleware context
    std::string user_id = auth_context.user_id;

    if (user_id.empty()) {
        res.code = 403;
        res.write("Unauthorized access: user_id missing.");
        res.end();
        return;
    }

    handleDeleteReview(req, res, mongoClient, reviewId, listingId, user_id); });
}

template void defineReviewsRoutes<MyApp>(MyApp &app, MongoClient &mongoClient);
