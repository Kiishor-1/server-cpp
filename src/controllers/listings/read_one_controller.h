#ifndef READ_ONE_CONTROLLER_H
#define READ_ONE_CONTROLLER_H

#include "crow.h"
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleReadOneListing(const crow::request &req, crow::response &res, const std::string& id, MongoClient &mongoClient);

#endif // READ_ONE_CONTROLLER_H
