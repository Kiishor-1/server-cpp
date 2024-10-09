#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include "crow.h"
#include "database/mongo_client.h"

void handleGetUserProfile(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &userId);

#endif // USER_PROFILE_H
