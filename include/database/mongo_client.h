// include/database/mongo_client.h
// #pragma once

// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>
// #include <mongocxx/uri.hpp>
// #include <bsoncxx/json.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <mongocxx/stdx.hpp>  // Add this line
// // #include "../stdx_optional.hpp"
// #include <memory>

// class MongoClient {
// public:
//     MongoClient(const std::string& uri);
//     mongocxx::database getDatabase(std::string& db_name);

// private:
//     mongocxx::instance instance_;
//     std::shared_ptr<mongocxx::client> client_;
// };


#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/stdx.hpp>
#include <memory>

class MongoClient {
public:
    MongoClient(const std::string& uri);
    mongocxx::database getDatabase(const std::string& db_name); // Changed to const std::string&

private:
    mongocxx::instance instance_;
    std::shared_ptr<mongocxx::client> client_;
};
