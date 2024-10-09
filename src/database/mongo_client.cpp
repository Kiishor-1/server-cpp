// #include "database/mongo_client.h"
// #include <iostream> // For debugging output
// #include <mongocxx/exception/exception.hpp>

// mongocxx::instance MongoClient::_instance{}; // Initialize the static instance

// MongoClient::MongoClient(const std::string &uri)
//     : _client(mongocxx::uri{uri})
// {
//     // Initialization logging
//     std::cout << "MongoClient initialized with URI: " << uri << std::endl;
// }

// mongocxx::collection MongoClient::getCollection(const std::string &database, const std::string &collection)
// {
//     try
//     {
//         auto db = _client[database];
//         return db[collection];
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error getting collection from database: " << database
//                   << ", collection: " << collection << std::endl;
//         std::cerr << "Exception: " << e.what() << std::endl;
//         throw; // Rethrow the exception after logging
//     }
// }

#include "database/mongo_client.h"
#include <iostream>
#include <mongocxx/exception/exception.hpp>

mongocxx::instance MongoClient::_instance{};

MongoClient::MongoClient(const std::string &uri)
    : _uri(uri) // Store the URI for creating new clients
{
    std::cout << "MongoClient initialized with URI: " << uri << std::endl;
}

mongocxx::client MongoClient::createClient()
{
    return mongocxx::client(mongocxx::uri{_uri});
}

mongocxx::collection MongoClient::getCollection(const std::string &database, const std::string &collection)
{
    try
    {
        auto client = createClient(); // Create a new client
        auto db = client[database];
        return db[collection];
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error getting collection from database: " << database
                  << ", collection: " << collection << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    }
}
