// is_logged_in.h
#ifndef IS_LOGGED_IN_H
#define IS_LOGGED_IN_H

#include "crow.h"

class isLoggedIn {
public:
    struct context {};

    void before_handle(const crow::request& req, crow::response& res, context& ctx) {
        // Here you would typically check a session or a token to see if the user is logged in
        bool loggedIn = true; // Replace with actual logic

        if (!loggedIn) {
            res.code = 401; // Unauthorized
            res.end("User not logged in");
        }
    }

    void after_handle(const crow::request& req, crow::response& res, context& ctx) {
        // No post-processing required
    }
};

#endif // IS_LOGGED_IN_H
