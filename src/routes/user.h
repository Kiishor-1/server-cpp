#ifndef USER_H
#define USER_H

#include "crow.h"
#include "database/mongo_client.h"

template <class AppType>
void defineUserRoutes(AppType& app, MongoClient& mongoClient);

#endif // USER_H
