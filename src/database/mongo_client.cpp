#include "database/mongo_client.h"
#include <iostream> // For debugging output
#include <mongocxx/exception/exception.hpp>

mongocxx::instance MongoClient::_instance{}; // Initialize the static instance

MongoClient::MongoClient(const std::string &uri)
    : _client(mongocxx::uri{uri})
{
    // Initialization logging
    std::cout << "MongoClient initialized with URI: " << uri << std::endl;
}

mongocxx::collection MongoClient::getCollection(const std::string &database, const std::string &collection)
{
    try
    {
        auto db = _client[database];
        return db[collection];
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error getting collection from database: " << database
                  << ", collection: " << collection << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        throw; // Rethrow the exception after logging
    }
}
