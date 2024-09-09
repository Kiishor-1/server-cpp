// validate_user.h
#ifndef VALIDATE_USER_H
#define VALIDATE_USER_H

#include "crow.h"

class validateUser {
public:
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // Validate user fields in the request
        bool valid = true; // Replace with actual validation logic

        if (!valid) {
            res.code = 400; // Bad Request
            res.end("Invalid user data");
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // No post-processing required
    }
};

#endif // VALIDATE_USER_H
