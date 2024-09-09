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
void defineListingsRoutes(AppType &app, MongoClient& mongoClient) {
    auto collection = mongoClient.getCollection("wanderlust2", "listings");
    if (!collection) {
        throw std::runtime_error("Failed to get collection in defineListingsRoutes");
    }


    CROW_ROUTE(app, "/create").methods(crow::HTTPMethod::Post)([&mongoClient](const crow::request &req, crow::response &res) {
        handleCreateListing(req, res, mongoClient);
    });

    CROW_ROUTE(app, "/listings").methods(crow::HTTPMethod::Get)([&mongoClient](const crow::request &req, crow::response &res) {
        handleReadListings(req, res, mongoClient);
    });

    CROW_ROUTE(app, "/listings/<string>").methods(crow::HTTPMethod::Get)([&mongoClient](const crow::request &req, crow::response &res, const std::string& id) {
        handleReadOneListing(req, res, id, mongoClient);
    });

    CROW_ROUTE(app, "/update/<string>").methods(crow::HTTPMethod::Post)([&mongoClient](const crow::request &req, crow::response &res, const std::string& id) {
        handleUpdateListing(req, res, id, mongoClient);
    });

    CROW_ROUTE(app, "/delete/<string>").methods(crow::HTTPMethod::Delete)([&mongoClient](const crow::request &req, crow::response &res, const std::string& id) {
        handleDeleteListing(req, res, id, mongoClient);
    });
}

template void defineListingsRoutes<MyApp>(MyApp &app,MongoClient& mongoClient);