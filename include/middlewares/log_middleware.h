// log_middleware.h
#ifndef LOG_MIDDLEWARE_H
#define LOG_MIDDLEWARE_H

#include "crow.h"
#include <iostream>

class LogMiddleware {
public:
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        std::cout << "Request received" << std::endl;
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // No post-processing required for this middleware
    }
};

#endif // LOG_MIDDLEWARE_H
