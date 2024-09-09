#ifndef DELETE_CONTROLLER_H
#define DELETE_CONTROLLER_H

#include "crow.h"
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleDeleteListing(const crow::request &req, crow::response &res, const std::string& id, MongoClient &mongoClient);

#endif // DELETE_CONTROLLER_H

