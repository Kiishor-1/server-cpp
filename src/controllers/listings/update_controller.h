#ifndef UPDATE_CONTROLLER_H
#define UPDATE_CONTROLLER_H

#include "crow.h"
#include <mongocxx/client.hpp>

void handleUpdateListing(const crow::request &req, crow::response &res, const std::string& id, mongocxx::collection &collection);

#endif // UPDATE_CONTROLLER_H
