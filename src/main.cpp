#include "crow.h"
#include "dotenv.h"
#include "routes/listings.h"
#include "../include/middleware.h"
#include "database/mongo_client.h"
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>

int main() {
    // Check if we are in a development environment
    const char* env = std::getenv("ENVIRONMENT");
    if (!env || std::string(env) == "development") {
        dotenv::load_dotenv("../.env");
    }

    const char* mongo_uri = std::getenv("MONGO_URI");
    if (!mongo_uri) {
        std::cerr << "Error loading MONGO_URI from environment" << std::endl;
        return 1;
    }

    try {
        MongoClient mongoClient{mongo_uri};
        std::cout << "MONGO_URI: " << mongo_uri << std::endl;
        auto collection = mongoClient.getCollection("wanderlust2", "listings");
        std::cout << "Successfully obtained collection" << std::endl;

        MyApp app;

        // Define routes
        defineListingsRoutes(app, mongoClient);

        // Default route to handle non-existent routes
        CROW_ROUTE(app, "/")
        ([]() {
            return crow::response(200, "Standard root");
        });

        CROW_ROUTE(app, "/*")
        ([]() {
            return crow::response(404, "Route not found");
        });

        app.port(8080).multithreaded().run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}









// #include "crow.h"
// #include "dotenv.h"
// #include "routes/listings.h"
// #include "../include/middleware.h"
// #include "database/mongo_client.h"
// #include<mongocxx/exception/exception.hpp>
// #include <bsoncxx/json.hpp>


// int main() {
//     // Check if we are in a development environment
//     const char* env = std::getenv("ENVIRONMENT");
//     if (!env || std::string(env) == "development") {
//         dotenv::load_dotenv("../.env");
//     }

//     const char* mongo_uri = std::getenv("MONGO_URI");
//     if (!mongo_uri) {
//         std::cerr << "Error loading MONGO_URI from environment" << std::endl;
//         return 1;
//     }

//     MongoClient mongoClient{mongo_uri};
   
//     std::cout << "MONGO_URI: " << mongo_uri << std::endl;
//     // auto collection = mongoClient.getCollection("wanderlust2", "listings");

//     // In main.cpp after initializing mongoClient
//     try {
//         auto collection = mongoClient.getCollection("wanderlust2", "listings");
//         if (!collection) {
//             throw std::runtime_error("Failed to get collection");
//         }
//         std::cout << "Successfully obtained collection" << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }


//     MyApp app;

//     // Define routes
//     defineListingsRoutes(app,mongoClient);
   
//     // Default route to handle non-existent routes
//     CROW_ROUTE(app, "/")
//     ([]() {
//         return crow::response(200, "Standard root");
//     });
//     CROW_ROUTE(app, "/*")
//     ([]() {
//         return crow::response(404, "Route not found");
//     });

//     app.port(8080).multithreaded().run();
//     return 0;
// }

// #include "crow.h"
// #include "dotenv.h"
// #include <bsoncxx/json.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>
// #include <mongocxx/uri.hpp>
// #include <iostream>
// #include <mongocxx/exception/exception.hpp>
// #include <bsoncxx/types.hpp>
// #include <bsoncxx/types/value.hpp>
// #include <bsoncxx/types/bson_value/view.hpp>
// #include "routes/listings.h"
// // #include "routes/user.h"

// #include "../include/middleware.h" // Adjust the path as needed

// int main()
// {
//     // Check if we are in a development environment
//     const char *env = std::getenv("ENVIRONMENT");
//     if (!env || std::string(env) == "development")
//     {
//         dotenv::load_dotenv("../.env");
//     }

//     const char *mongo_uri = std::getenv("MONGO_URI");
//     if (!mongo_uri)
//     {
//         std::cerr << "Error loading MONGO_URI from environment" << std::endl;
//         return 1;
//     }

//     std::cout << "MONGO_URI: " << mongo_uri << std::endl;

//     MyApp app;

//     // defineUserRoutes(app);
//     // defineListingsRoutes(app);
//     mongocxx::instance instance{};
//     mongocxx::client client{mongocxx::uri{mongo_uri}};
//     auto collection = client["wanderlust2"]["listings"];

//     CROW_ROUTE(app, "/create").methods(crow::HTTPMethod::Post)([&collection](const crow::request &req)
//                                                                {
//         try {
//             auto json_body = crow::json::load(req.body);

//             if (!json_body) {
//                 return crow::response(400, "Invalid JSON format");
//             }

//             bsoncxx::builder::stream::document document{};
//             for (const auto& item : json_body) {
//                 if (item.t() == crow::json::type::String) {
//                     document << item.key() << item.s();
//                 } else if (item.t() == crow::json::type::Number) {
//                     document << item.key() << item.i();
//                 }
//             }

//             collection.insert_one(document.view());
//             return crow::response(200, "Listing created");
//         } catch (const mongocxx::exception& e) {
//             return crow::response(500, e.what());
//         } catch (const std::exception& e) {
//             return crow::response(500, e.what());
//         } });

//     // READ ALL
//     CROW_ROUTE(app, "/listings")
//     ([&collection](const crow::request &req, crow::response &res)
//      {
//         auto cursor = collection.find({});
//         crow::json::wvalue listings;
//         std::vector<crow::json::wvalue> listings_array;

//         for (auto&& doc : cursor) {
//             listings_array.push_back(crow::json::load(bsoncxx::to_json(doc)));
//         }
//         listings["listings"] = std::move(listings_array);

//         res.write(crow::json::dump(listings));
//         res.end(); });

//     // READ ONE
//     CROW_ROUTE(app, "/listings/<string>")
//     ([&collection](const std::string &id)
//      {
//         try {
//             bsoncxx::oid oid{id};
//             auto maybe_result = collection.find_one(bsoncxx::builder::stream::document{} << "_id" << oid << bsoncxx::builder::stream::finalize);

//             if (maybe_result) {
//                 return crow::response{bsoncxx::to_json(*maybe_result)};
//             } else {
//                 return crow::response(404, "Listing not found");
//             }
//         } catch (const mongocxx::exception& e) {
//             return crow::response(500, e.what());
//         } catch (const std::exception& e) {
//             return crow::response(500, e.what());
//         } });

//     // UPDATE
//     CROW_ROUTE(app, "/update/<string>").methods(crow::HTTPMethod::Post)([&collection](const crow::request &req, const std::string &id)
//                                                                         {
//         try {
//             auto json_body = crow::json::load(req.body);

//             if (!json_body) {
//                 return crow::response(400, "Invalid JSON format");
//             }

//             bsoncxx::oid oid{id};
//             bsoncxx::builder::stream::document filter{};
//             filter << "_id" << oid;

//             bsoncxx::builder::stream::document update_doc{};
//             update_doc << "$set" << bsoncxx::builder::stream::open_document;
//             for (const auto& item : json_body) {
//                 if (item.t() == crow::json::type::String) {
//                     update_doc << item.key() << item.s();
//                 } else if (item.t() == crow::json::type::Number) {
//                     update_doc << item.key() << item.i();
//                 }
//             }
//             update_doc << bsoncxx::builder::stream::close_document;

//             auto result = collection.update_one(filter.view(), update_doc.view());
//             if (result && result->modified_count() == 1) {
//                 return crow::response(200, "Listing updated");
//             } else {
//                 return crow::response(404, "Listing not found or no changes made");
//             }
//         } catch (const mongocxx::exception& e) {
//             return crow::response(500, e.what());
//         } catch (const std::exception& e) {
//             return crow::response(500, e.what());
//         } });

//     // DELETE
//     CROW_ROUTE(app, "/delete/<string>").methods(crow::HTTPMethod::Delete)([&collection](const std::string &id)
//                                                                           {
//         try {
//             bsoncxx::oid oid{id};
//             bsoncxx::builder::stream::document filter{};
//             filter << "_id" << oid;

//             auto result = collection.delete_one(filter.view());
//             if (result && result->deleted_count() == 1) {
//                 return crow::response(200, "Listing deleted");
//             } else {
//                 return crow::response(404, "Listing not found");
//             }
//         } catch (const mongocxx::exception& e) {
//             return crow::response(500, e.what());
//         } catch (const std::exception& e) {
//             return crow::response(500, e.what());
//         } });

//     // Default route to handle non-existent routes
//     CROW_ROUTE(app, "/*")
//     ([]()
//      { return crow::response(404, "Route not found"); });

//     app.port(8080).multithreaded().run();
//     return 0;
// }