#include "read_controller.h"
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleReadListings(const crow::request &req, crow::response &res, MongoClient &mongoClient)
{
    auto collection = mongoClient.getCollection("wanderlust2", "listings");
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
        auto cursor = collection.find({});
        crow::json::wvalue listings;
        std::vector<crow::json::wvalue> listings_array;

        for (auto &&doc : cursor)
        {
            listings_array.push_back(crow::json::load(bsoncxx::to_json(doc)));
        }
        listings["listings"] = std::move(listings_array);

        res.write(crow::json::dump(listings));
    }
    catch (const mongocxx::exception &e)
    {
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to read listings: ");
        res.write(e.what());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Failed to read listings: ");
        res.write(e.what());
    }
    catch (...)
    {
        std::cerr << "Unknown exception" << std::endl;
        res.code = 500;
        res.write("Failed to read listings: Unknown error");
    }
    res.end();
}

// // read_controller.cpp

// #include "read_controller.h"
// #include <bsoncxx/json.hpp>
// #include <crow/json.h>
// #include <mongocxx/exception/exception.hpp>
// #include <mongocxx/collection.hpp>

// void handleReadListings(const crow::request &req, crow::response &res, mongocxx::collection& collection)
// {
//     std::cout << "Entered handleReadListings" << std::endl;
//     if(!collection) {
//         std::cout << "No valid collection" << std::endl;
//         res.code = 500;
//         res.write("Invalid collection");
//         res.end();
//         return;
//     }
//     std::cout << "Valid collection" << std::endl;
//     try
//     {
//         std::cout << "Executing find on collection" << std::endl;
//         auto cursor = collection.find({}, { mongocxx::options::find{}.no_cursor_timeout(true) });
//         std::cout << "Find executed successfully" << std::endl;
//         crow::json::wvalue listings;
//         std::cout<<"check 2"<<std::endl;
//         std::vector<crow::json::wvalue> listings_array;
//         std::cout<<"check 3"<<std::endl;

//         for (auto &&doc : cursor)
//         {
//             listings_array.push_back(crow::json::load(bsoncxx::to_json(doc)));
//         }
//         std::cout<<"check 4"<<std::endl;
//         listings["listings"] = std::move(listings_array);
//         std::cout<<"check 5"<<std::endl;

//         res.write(crow::json::dump(listings));
//     }
//    catch (const mongocxx::exception& e) {
//         res.code = 500;
//         res.write("Failed to read listings: ");
//         res.write(e.what());
//     }
//     catch (const std::exception& e) {
//         res.code = 500;
//         res.write("Failed to read listings: ");
//         res.write(e.what());
//     }catch (...) {
//         res.code = 500;
//         res.write("Failed to read listings: Unknown error");
//     }
//     res.end();
// };

// void handleReadListings(const crow::request& req, crow::response& res, mongocxx::collection& collection) {
//     std::cout<<"executed"<<std::endl;
//     try {
//         auto cursor = collection.find({});
//         crow::json::wvalue listings;
//         std::vector<crow::json::wvalue> listings_array;

//         for (auto&& doc : cursor) {
//             crow::json::wvalue listing = crow::json::load(bsoncxx::to_json(doc));
//             listings_array.push_back(std::move(listing));
//         }

//         listings["listings"] = std::move(listings_array);

//         res.code = 200;
//         res.write(crow::json::dump(listings));
//     } catch (const mongocxx::exception& e) {
//         res.code = 500;
//         res.write("Failed to read listings: ");
//         res.write(e.what());
//     }
//     catch (const std::exception& e) {
//         res.code = 500;
//         res.write("Failed to read listings: ");
//         res.write(e.what());
//     }catch (...) {
//         res.code = 500;
//         res.write("Failed to read listings: Unknown error");
//     }
//     res.end();
// }
