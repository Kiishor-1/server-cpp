#ifndef CREATE_BOOKING_CONTROLLER_H
#define CREATE_BOOKING_CONTROLLER_H

#include <crow.h>
#include "database/mongo_client.h"

void handleInitBooking(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& listingId, const std::string& userId);

#endif
