#ifndef SIGNUP_H
#define SIGNUP_H

#include <crow.h>
#include "database/mongo_client.h"

void handleUserRegistration(const crow::request& req, crow::response& res, MongoClient& mongoClient);

#endif // SIGNUP_H
