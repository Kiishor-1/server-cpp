#include <openssl/ssl.h>
#include "crow.h"
#include "dotenv.h"
#include "routes/listings.h"
#include "routes/user.h"
#include "routes/reviews.h"
#include "routes/booking.h"
#include "routes/payment.h"
#include "../include/middleware.h"
#include "database/mongo_client.h"
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>

// Checkpoint: Ensure Windows headers are included properly

bool loadDotenv(const std::string& filename) {
    std::ifstream file(filename);
    if (file.good()) {
        dotenv::load_dotenv(filename.c_str());
        return true;
    }
    return false;
}

int main() {
    try {
        // Try to load the .env file if it exists
        bool dotenvLoaded = loadDotenv("../.env");
        if (dotenvLoaded) {
            std::cout << ".env file loaded successfully" << std::endl;
        } else {
            std::cout << ".env file not found. Using environment variables directly." << std::endl;
        }

        const char* mongo_uri = std::getenv("MONGO_URI");
        if (!mongo_uri) {
            std::cerr << "MONGO_URI is not set in environment variables." << std::endl;
            return 1;
        }

        // Log the MongoDB URI for debugging
        std::cout << "MONGO_URI: " << mongo_uri << std::endl;

        // Initialize MongoDB client
        MongoClient mongoClient{mongo_uri};
        auto collection = mongoClient.getCollection("wanderlust2", "listings");
        std::cout << "Successfully obtained collection" << std::endl;

        // Initialize Crow application
        MyApp app;

        // Define routes with error handling
        try {
            defineListingsRoutes(app, mongoClient);  // Define all routes related to listings
            defineUserRoutes(app,mongoClient);
            defineReviewsRoutes(app, mongoClient);
            defineBookingRoutes(app, mongoClient);
            definePaymentRoutes(app, mongoClient);
        } catch (const std::exception &e) {
            std::cerr << "Error defining routes: " << e.what() << std::endl;
            return 1;
        }

        // Default route to handle non-existent routes
        CROW_ROUTE(app, "/")([]() {
            return crow::response(200, "Standard root");
        });

        CROW_ROUTE(app, "/*")([]() {
            return crow::response(404, "Route not found");
        });

        // Start server
        app.port(8080).multithreaded().run();

    } catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Exception: " << e.what() << std::endl;
        return 1; // Exit with error code
    } catch (const std::exception& e) {
        std::cerr << "General Exception: " << e.what() << std::endl;
        return 1; // Exit with error code
    }

    return 0; // Success exit
}


