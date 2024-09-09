#ifndef CREATE_CONTROLLER_H
#define CREATE_CONTROLLER_H

#include <crow.h>
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"

void handleCreateListing(const crow::request& req, crow::response& res, MongoClient &mongoClient);

#endif // CREATE_CONTROLLER_H



