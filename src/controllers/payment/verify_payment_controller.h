#ifndef VERIFY_PAYMENT_CONTROLLER_H
#define VERIFY_PAYMENT_CONTROLLER_H

#include <crow.h>
#include "database/mongo_client.h"

void handleVerifyPayment(const crow::request& req, crow::response& res, MongoClient& mongoClient);

#endif
