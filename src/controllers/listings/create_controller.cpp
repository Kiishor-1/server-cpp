// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>
// #include <mongocxx/uri.hpp>
// #include <mongocxx/collection.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <bsoncxx/json.hpp>
// #include <crow/json.h>
// #include <crow/app.h>
// #include <stdexcept>

// // Function to convert crow::json::rvalue to bsoncxx::document::value
// bsoncxx::document::value convert_to_bson(const crow::json::rvalue& json_value) {
//     bsoncxx::builder::stream::document doc{};

//     for (const auto& item : json_value) {
//         const std::string& key = item.key();
//         const auto& value = item;

//         if (value.t() == crow::json::type::String) {
//             doc << key << value.s();
//         } else if (value.t() == crow::json::type::Number) {
//             doc << key << value.d();
//         } else if (value.t() == crow::json::type::True) {
//             doc << key << true;
//         } else if (value.t() == crow::json::type::False) {
//             doc << key << false;
//         } else if (value.t() == crow::json::type::List) {
//             bsoncxx::builder::basic::array array_builder{};
//             for (const auto& array_item : value) {
//                 array_builder.append(convert_to_bson(array_item).view());
//             }
//             doc << key << array_builder;
//         } else if (value.t() == crow::json::type::Object) {
//             doc << key << convert_to_bson(value).view();
//         } else {
//             throw std::runtime_error("Unsupported JSON value type");
//         }
//     }

//     return doc.extract();
// }

// void handleCreateListing(const crow::request& req, crow::response& res, mongocxx::collection& collection) {
//     auto body = crow::json::load(req.body);
//     if (!body) {
//         res.code = 400;
//         res.write("Invalid JSON");
//         res.end();
//         return;
//     }

//     try {
//         bsoncxx::document::value doc = convert_to_bson(body);
//         collection.insert_one(doc.view());
//         res.code = 200;
//         res.write("Listing created");
//     } catch (const std::exception& e) {
//         res.code = 500;
//         res.write("Failed to create listing: ");
//         res.write(e.what());
//     } catch (const std::exception& e) {
//         res.code = 500;
//         res.write("Failed to create listing: ");
//         res.write(e.what());
//     }

//     res.end();
// }


#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <crow/app.h>
#include <iostream>
#include <mongocxx/exception/exception.hpp>
#include "database/mongo_client.h"


void handleCreateListing(const crow::request& req, crow::response& res, MongoClient &mongoClient) {
    auto collection = mongoClient.getCollection("wanderlust2", "listings");
    if (!collection)
    {
        std::cerr << "No valid collection" << std::endl;
        res.code = 500;
        res.write("No valid collection");
        res.end();
        return;
    }
    try {
        auto json_body = crow::json::load(req.body);

        if (!json_body) {
            res.code = 400;
            res.write("Invalid JSON");
            res.end();
            return;
        }

        bsoncxx::builder::stream::document document{};
        for (const auto& item : json_body) {
            if (item.t() == crow::json::type::String) {
                document << item.key() << item.s();
            } else if (item.t() == crow::json::type::Number) {
                document << item.key() << item.i();
            }
        }

        collection.insert_one(document.view());
        res.code = 200;
        res.write("Listing created");
        res.end();
    } catch (const mongocxx::exception& e) {
        res.code = 500;
        res.write("Failed to create listing: ");
        res.write(e.what());
        res.end();
    } catch (const std::exception& e) {
        res.code = 500;
        res.write("Failed to create listing: ");
        res.write(e.what());
        res.end();
    }
}


