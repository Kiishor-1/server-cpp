#ifndef LISTINGS_H
#define LISTINGS_H

#include "crow.h"
#include "database/mongo_client.h"


template <typename AppType>
void defineListingsRoutes(AppType& app, MongoClient& mongoClient);

// void defineListingsRoutes(crow::SimpleApp& app);


#endif // LISTINGS_H