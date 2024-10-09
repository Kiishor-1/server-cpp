#ifndef CREATE_CONTROLLER_H
#define CREATE_CONTROLLER_H

#include <crow.h>
#include <mongocxx/collection.hpp>
#include "database/mongo_client.h"
#include "../include/middlewares/auth_middleware.h"

void handleCreateListing(const crow::request& req, crow::response& res, MongoClient &mongoClient,const std::string& user_id);

#endif // CREATE_CONTROLLER_H



