// validate_listing.h
#ifndef VALIDATE_LISTING_H
#define VALIDATE_LISTING_H

#include "crow.h"

class validateListing {
public:
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // Validate listing fields in the request
        bool valid = true; // Replace with actual validation logic

        if (!valid) {
            res.code = 400; // Bad Request
            res.end("Invalid listing data");
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // No post-processing required
    }
};

#endif // VALIDATE_LISTING_H
