#include "read_controller.h"
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp> // Required for handling arrays
#include "database/mongo_client.h"

void handleReadListings(const crow::request &req, crow::response &res, MongoClient &mongoClient)
{
    // auto collection = mongoClient.getCollection("wanderlust2", "listings");
    auto client = mongoClient.createClient(); // Create a new client for each request
    auto collection = client["wanderlust2"]["listings"];
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
        // Create a BSON filter document
        bsoncxx::builder::stream::document filter_builder;
        bool filter_applied = false;

        // Access query parameters directly using req.url_params
        const char *search = req.url_params.get("search");
        const char *category = req.url_params.get("category");
        const char *minPrice = req.url_params.get("minPrice");
        const char *maxPrice = req.url_params.get("maxPrice");

        // Search functionality
        if (search)
        {
            std::string searchQuery = search;
            std::cout << searchQuery << std::endl;
            filter_builder << "$or" << bsoncxx::builder::stream::open_array
                           << bsoncxx::builder::stream::open_document
                           << "title" << bsoncxx::builder::stream::open_document
                           << "$regex" << searchQuery
                           << "$options" << "i" // Case-insensitive search
                           << bsoncxx::builder::stream::close_document
                           << bsoncxx::builder::stream::close_document
                           << bsoncxx::builder::stream::open_document
                           << "description" << bsoncxx::builder::stream::open_document
                           << "$regex" << searchQuery
                           << "$options" << "i" // Case-insensitive search
                           << bsoncxx::builder::stream::close_document
                           << bsoncxx::builder::stream::close_document
                           << bsoncxx::builder::stream::open_document
                           << "location" << bsoncxx::builder::stream::open_document
                           << "$regex" << searchQuery
                           << "$options" << "i" // Case-insensitive search
                           << bsoncxx::builder::stream::close_document
                           << bsoncxx::builder::stream::close_document
                           << bsoncxx::builder::stream::close_array;
            filter_applied = true;
        }

        // Category filter
        if (category && std::strlen(category) > 0) // Check if category is not empty
        {
            std::string categoryQuery = category;
            // Split the categories by comma
            std::vector<std::string> categoryList;
            std::stringstream ss(categoryQuery);
            std::string item;
            while (std::getline(ss, item, ','))
            {
                categoryList.push_back(item);
            }

            if (!categoryList.empty())
            {
                // Add category filter using array context
                auto category_array = bsoncxx::builder::stream::array{};
                for (const auto &cat : categoryList)
                {
                    category_array << cat;
                }

                filter_builder << "category" << bsoncxx::builder::stream::open_document
                               << "$in" << category_array
                               << bsoncxx::builder::stream::close_document;

                filter_applied = true;
            }
        }

        // Price range filter
        if (minPrice && maxPrice)
        {
            int minPriceValue = std::stoi(minPrice);
            int maxPriceValue = std::stoi(maxPrice);
            filter_builder << "price" << bsoncxx::builder::stream::open_document
                           << "$gte" << minPriceValue
                           << "$lte" << maxPriceValue
                           << bsoncxx::builder::stream::close_document;
            filter_applied = true;
        }

        // Construct the filter
        bsoncxx::document::value filter_doc = filter_applied
                                                  ? filter_builder << bsoncxx::builder::stream::finalize
                                                  : bsoncxx::builder::stream::document{} << bsoncxx::builder::stream::finalize;

        // Fetch listings from MongoDB based on filters
        auto cursor = collection.find(filter_doc.view());

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
