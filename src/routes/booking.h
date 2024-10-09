#ifndef BOOKING_ROUTES_H
#define BOOKING_ROUTES_H

#include <crow.h>
#include "database/mongo_client.h"

template <typename AppType>
void defineBookingRoutes(AppType& app, MongoClient& mongoClient);

#endif
