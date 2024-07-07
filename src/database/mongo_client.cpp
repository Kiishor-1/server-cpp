#include "database/mongo_client.h"
#include <iostream> // For debugging output

MongoClient::MongoClient(const std::string& uri)
    : _client(mongocxx::uri{uri}) {
    // Ensure proper initialization
    try {
        std::cout << "MongoClient initialized with URI: " << uri << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error initializing MongoClient: " << e.what() << std::endl;
        throw;
    }
}

mongocxx::collection MongoClient::getCollection(const std::string& database, const std::string& collection) {
    try {
        auto db = _client[database];
        // std::cout << "Database name: " << database << std::endl;
        // std::cout << "Collection name: " << collection << std::endl;
        return db[collection];
    } catch (const std::exception& e) {
        std::cerr << "Error getting collection: " << e.what() << std::endl;
        throw;
    }
}



















// #include "database/mongo_client.h"

// MongoClient::MongoClient(const std::string& uri)
//     : client(mongocxx::uri{uri}) {
// }

// mongocxx::collection MongoClient::getCollection(const std::string& database, const std::string& collection) {
//     return client[database][collection];
// }
