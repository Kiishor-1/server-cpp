#ifndef MONGO_CLIENT_H
#define MONGO_CLIENT_H

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <string>

class MongoClient {
public:
    explicit MongoClient(const std::string& uri); // Ensure explicit constructor
    mongocxx::collection getCollection(const std::string& database, const std::string& collection);

private:
    static mongocxx::instance _instance; // Ensure only one instance per application
    mongocxx::client _client;
};

#endif // MONGO_CLIENT_H



















// #ifndef MONGO_CLIENT_H
// #define MONGO_CLIENT_H

// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>

// class MongoClient {
// public:
//     MongoClient(const std::string& uri);
//     mongocxx::collection getCollection(const std::string& database, const std::string& collection);

// private:
//     mongocxx::instance instance; // Ensure only one instance per application
//     mongocxx::client client;
// };

// #endif // MONGO_CLIENT_H
