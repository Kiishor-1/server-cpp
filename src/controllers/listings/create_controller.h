#ifndef CREATE_CONTROLLER_H
#define CREATE_CONTROLLER_H

#include <crow.h>
#include <mongocxx/collection.hpp>

void handleCreateListing(const crow::request& req, crow::response& res, mongocxx::collection& collection);

#endif // CREATE_CONTROLLER_H



