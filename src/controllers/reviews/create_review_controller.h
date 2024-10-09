#ifndef CREATE_REVIEW_CONTROLLER_H
#define CREATE_REVIEW_CONTROLLER_H

#include <crow.h>
#include "database/mongo_client.h"

// Function declaration for creating a review
void handleCreateReview(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &user_id, const std::string &listingId);

#endif // CREATE_REVIEW_CONTROLLER_H
