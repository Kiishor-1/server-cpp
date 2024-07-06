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

// Undefine CROW_ROUTE to avoid redefinition warning
#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
#define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

const char* mongo_uri = std::getenv("MONGO_URI");
mongocxx::instance instance{};
mongocxx::client client{mongocxx::uri{mongo_uri}};
mongocxx::collection collection = client["wanderlust2"]["listings"];

template <typename AppType>
void defineListingsRoutes(AppType &app) {
    CROW_ROUTE(app, "/create").methods(crow::HTTPMethod::Post)([](const crow::request &req, crow::response &res) {
        handleCreateListing(req, res, collection);
    });

    CROW_ROUTE(app, "/listings").methods(crow::HTTPMethod::Get)([](const crow::request &req, crow::response &res) {
        handleReadListings(req, res, collection);
    });

    CROW_ROUTE(app, "/listings/<string>").methods(crow::HTTPMethod::Get)([](const crow::request &req, crow::response &res, const std::string& id) {
        handleReadOneListing(req, res, id, collection);
    });

    CROW_ROUTE(app, "/update/<string>").methods(crow::HTTPMethod::Post)([](const crow::request &req, crow::response &res, const std::string& id) {
        handleUpdateListing(req, res, id, collection);
    });

    CROW_ROUTE(app, "/delete/<string>").methods(crow::HTTPMethod::Delete)([](const crow::request &req, crow::response &res, const std::string& id) {
        handleDeleteListing(req, res, id, collection);
    });
}

template void defineListingsRoutes<MyApp>(MyApp &app);