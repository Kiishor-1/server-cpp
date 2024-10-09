#ifndef PAYMENT_ROUTES_H
#define PAYMENT_ROUTES_H

#include <crow.h>
#include "database/mongo_client.h"

template <typename AppType>
void definePaymentRoutes(AppType& app, MongoClient& mongoClient);

#endif
