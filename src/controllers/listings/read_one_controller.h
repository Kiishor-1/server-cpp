#ifndef READ_ONE_CONTROLLER_H
#define READ_ONE_CONTROLLER_H

#include "crow.h"
#include <mongocxx/client.hpp>

void handleReadOneListing(const crow::request &req, crow::response &res, const std::string& id, mongocxx::collection &collection);

#endif // READ_ONE_CONTROLLER_H
