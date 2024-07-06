#ifndef MIDDLEWARES_H
#define MIDDLEWARES_H

#include "crow.h"
#include <iostream>

template <typename... Middlewares>
class CrowApp : public crow::Crow<Middlewares...> {};

struct CORS {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        std::cout << "CORS headers set in before_handle for " << req.url << std::endl;

        // Explicitly handle preflight requests
        if (req.method == crow::HTTPMethod::Options) {
            res.code = 204; // No Content
            res.end();
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // Ensure headers are set for all responses
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        std::cout << "CORS headers set in after_handle for " << req.url << std::endl;
    }
};

struct LogMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        std::cout << "Request received: " << req.url << std::endl;
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {}
};

struct ErrorHandlerMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {}

    void after_handle(crow::request& req, crow::response& res, context& ctx) {}

    void handle_exception(const std::exception& e, crow::response& res) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Internal Server Error: " + std::string(e.what()));
        res.end();
    }
};

using MyApp = CrowApp<CORS, LogMiddleware, ErrorHandlerMiddleware>;

#endif // MIDDLEWARES_H