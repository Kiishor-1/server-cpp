#ifndef LISTINGS_H
#define LISTINGS_H

#include "crow.h"
// #include <mongocxx/client.hpp>
#include "database/mongo_client.h"


// Template declaration
template <typename AppType>
void defineListingsRoutes(AppType& app, MongoClient& mongoClient);

// void defineListingsRoutes(crow::SimpleApp& app);


#endif // LISTINGS_H