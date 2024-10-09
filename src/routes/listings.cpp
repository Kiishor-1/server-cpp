#include "listings.h"
#include "../controllers/listings/create_controller.h"
#include "../controllers/listings/read_controller.h"
#include "../controllers/listings/read_one_controller.h"
#include "../controllers/listings/update_controller.h"
#include "../controllers/listings/delete_controller.h"
#include "../../include/middleware.h"
#include "dotenv.h"
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <mongocxx/exception/exception.hpp>
// #include "../include/database/mongo_client.h"
#include "database/mongo_client.h"

// Undefine CROW_ROUTE to avoid redefinition warning
#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
#define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

template <typename AppType>
void defineListingsRoutes(AppType &app, MongoClient &mongoClient)
{
    auto collection = mongoClient.getCollection("wanderlust2", "listings");
    if (!collection)
    {
        throw std::runtime_error("Failed to get collection in defineListingsRoutes");
    }

    CROW_ROUTE(app, "/create").methods(crow::HTTPMethod::Post)([&app, &mongoClient](const crow::request &req, crow::response &res)
                                                               {
        auto& auth_context = app.template get_context<AuthMiddleware>(req);  // Get AuthMiddleware context
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }

        handleCreateListing(req, res, mongoClient, user_id); 
    });

    CROW_ROUTE(app, "/listings").methods(crow::HTTPMethod::Get)([&mongoClient](const crow::request &req, crow::response &res)
                                                                { handleReadListings(req, res, mongoClient); });

    CROW_ROUTE(app, "/listings/<string>").methods(crow::HTTPMethod::Get)([&mongoClient](const crow::request &req, crow::response &res, const std::string &id)
                                                                         { handleReadOneListing(req, res, id, mongoClient); });

    CROW_ROUTE(app, "/update/<string>").methods(crow::HTTPMethod::Post)([&app, &mongoClient](const crow::request &req, crow::response &res, const std::string &id){
        auto& auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }
        handleUpdateListing(req, res ,id ,mongoClient, user_id); 
    });

    CROW_ROUTE(app, "/delete/<string>").methods(crow::HTTPMethod::Delete)([&app, &mongoClient](const crow::request &req, crow::response &res, const std::string &id){
        auto& auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }
        handleDeleteListing(req, res, id, mongoClient, user_id);
    });

    
}

template void defineListingsRoutes<MyApp>(MyApp &app, MongoClient &mongoClient);