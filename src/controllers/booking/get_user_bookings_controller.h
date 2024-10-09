#ifndef GET_USER_BOOKINGS_CONTROLLER_H
#define GET_USER_BOOKINGS_CONTROLLER_H

#include "crow.h"
#include "database/mongo_client.h"

// Function to handle fetching user bookings
void handleGetUserBookings(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& userId);

#endif // GET_USER_BOOKINGS_CONTROLLER_H
