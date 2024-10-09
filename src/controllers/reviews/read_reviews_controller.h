#ifndef READ_REVIEWS_CONTROLLER_H
#define READ_REVIEWS_CONTROLLER_H

#include <crow.h>
#include "database/mongo_client.h"

// Function declaration for reading reviews of a listing
void handleReadReviews(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &listingId);

#endif // READ_REVIEWS_CONTROLLER_H