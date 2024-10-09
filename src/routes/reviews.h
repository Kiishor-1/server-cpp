#ifndef REVIEWS_H
#define REVIEWS_H

#include <crow.h>
#include "database/mongo_client.h"

// Function to define review routes
template <typename AppType>
void defineReviewsRoutes(AppType &app, MongoClient &mongoClient);

#endif // REVIEWS_H
