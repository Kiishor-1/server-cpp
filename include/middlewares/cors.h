// cors.h
#ifndef CORS_H
#define CORS_H

#include "crow.h"

class CORS {
public:
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // Set CORS headers to allow all origins and methods
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        // Handle preflight OPTIONS requests
        if (req.method == crow::HTTPMethod::Options) {
            res.end();
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // No post-processing required for this middleware
    }
};

#endif // CORS_H
