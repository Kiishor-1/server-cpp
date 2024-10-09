#ifndef INITIATE_PAYMENT_CONTROLLER_H
#define INITIATE_PAYMENT_CONTROLLER_H

#include <crow.h>
#include "database/mongo_client.h"

void handleInitiatePayment(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string &user_id);

#endif
