// // src/database/mongo_client.cpp
// #include "database/mongo_client.h"
// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>
// #include <mongocxx/uri.hpp>
// #include <bsoncxx/json.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <mongocxx/stdx.hpp>  // Ensure this line is present
// // #include "../../include/stdx_optional.hpp"
// #include <memory>


// MongoClient::MongoClient(const std::string& uri) 
//     : client_(std::make_shared<mongocxx::client>(mongocxx::uri{uri})) {}

// mongocxx::database MongoClient::getDatabase( const std::string& db_name) {
//     return client_->database(db_name);
// }



#include "database/mongo_client.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/stdx.hpp>
#include <memory>

MongoClient::MongoClient(const std::string& uri) 
    : client_(std::make_shared<mongocxx::client>(mongocxx::uri{uri})) {}

mongocxx::database MongoClient::getDatabase(const std::string& db_name) { // Changed to const std::string&
    return client_->database(db_name);
}
