// is_author.h
#ifndef IS_AUTHOR_H
#define IS_AUTHOR_H

#include "crow.h"

class isAuthor {
public:
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // Here you would check if the user is the author of the listing
        bool isAuthor = false; // Replace with actual logic

        if (!isAuthor) {
            res.code = 403; // Forbidden
            res.end("User is not the author of the listing");
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // No post-processing required
    }
};

#endif // IS_AUTHOR_H
