#ifndef UPDATE_CONTROLLER_H
#define UPDATE_CONTROLLER_H

#include "crow.h"
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleUpdateListing(const crow::request &req, crow::response &res, const std::string &listing_id, MongoClient &mongoClient, const std::string &user_id);

#endif // UPDATE_CONTROLLER_H
