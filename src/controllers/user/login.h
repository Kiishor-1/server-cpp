#ifndef LOGIN_H
#define LOGIN_H

#include <crow.h>
#include "database/mongo_client.h"

void handleUserLogin(const crow::request& req, crow::response& res, MongoClient& mongoClient);

#endif // LOGIN_H
