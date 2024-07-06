#ifndef DELETE_CONTROLLER_H
#define DELETE_CONTROLLER_H

#include "crow.h"
#include <mongocxx/client.hpp>

void handleDeleteListing(const crow::request &req, crow::response &res, const std::string& id, mongocxx::collection &collection);

#endif // DELETE_CONTROLLER_H

