#ifndef READ_CONTROLLER_H
#define READ_CONTROLLER_H

#include "crow.h"
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleReadListings(const crow::request &req, crow::response &res, MongoClient& mongoClient);

#endif // READ_CONTROLLER_H
