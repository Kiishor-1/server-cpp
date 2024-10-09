#ifndef DELETE_REVIEW_CONTROLLER_H
#define DELETE_REVIEW_CONTROLLER_H

#include <crow.h>
#include "database/mongo_client.h"

// Function declaration for deleting a review
void handleDeleteReview(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &reviewId, const std::string &listingId, const std::string &userId);

#endif // DELETE_REVIEW_CONTROLLER_H
