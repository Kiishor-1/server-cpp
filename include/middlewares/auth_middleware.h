#ifndef AUTH_MIDDLEWARE_H
#define AUTH_MIDDLEWARE_H

#include "jwt-cpp/jwt.h"
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <crow.h>

class AuthMiddleware
{
public:
    struct context
    {
        std::string user_id; // Store user_id in the context for use throughout the request
    };

    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        // List of route regex patterns that require authentication
        const std::vector<std::regex> protectedRoutes = {
            std::regex("^/create$"),
            std::regex("^/payments/initiate$"),
            std::regex("^/payments/verify$"),
            std::regex("^/user/profile$"),
            std::regex("^/update/[^/]+$"),                // Matches /update/<string>
            std::regex("^/delete/[^/]+$"),                // Matches /delete/<string>
            std::regex("^/listings/[^/]+/reviews$"),      // Matches /listings/<string>/reviews
            std::regex("^/listings/[^/]+/reviews/[^/]+$"), // Matches /listings/<string>/reviews/<string>
            std::regex("^/bookings/user$"),
            std::regex("^/bookings/[^/]+/create$"), // Matches /bookings/<string>/create
            std::regex("^/bookings/[^/]+/delete$") // Matches /bookings/<string>/delete
        };

        // Check if the current request path matches any protected route
        bool isProtectedRoute = false;
        for (const auto &route : protectedRoutes)
        {
            if (std::regex_match(req.url, route))
            {
                isProtectedRoute = true;
                break;
            }
        }

        // If the route is protected, process authentication
        if (isProtectedRoute)
        {
            const auto authHeader = req.get_header_value("Authorization");
            std::cout << "Authorization header: " << authHeader << std::endl;

            // Validate Authorization header
            if (authHeader.empty())
            {
                res.code = 401; // Unauthorized
                res.set_header("Content-Type", "application/json");
                res.write(R"({"message": "Please login to continue"})");
                res.end();
                return;
            }


            // Check if the token is in Bearer format
            std::regex bearerRegex("^Bearer (.+)$");
            std::smatch match;
            if (!std::regex_search(authHeader, match, bearerRegex))
            {
                res.code = 401;
                res.write("Unauthorized: Invalid token format");
                res.end();
                return;
            }

            std::string token = match[1].str();
            std::string userId;
            if (!validate_token_and_get_user_id(token, userId))
            {
                res.code = 401; // Unauthorized
                res.write("Unauthorized: Invalid token");
                res.end();
                return;
            }

            if (userId.empty())
            {
                res.code = 401; // Unauthorized
                res.write("Unauthorized: Missing user ID in token");
                res.end();
                return;
            }

            // Store the user_id in the request's context
            ctx.user_id = userId;
            std::cout << "User ID stored in context: " << ctx.user_id << std::endl;
        }
    }

    void after_handle(crow::request &req, crow::response &res, context &ctx)
    {
        // Post-processing can go here if needed
    }

private:
    // Token validation function and extraction of user_id
    bool validate_token_and_get_user_id(const std::string &token, std::string &userId)
    {
        try
        {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{get_secret_key()})
                                .with_issuer("auth-server");
            verifier.verify(decoded);

            // Extract user_id from the token's payload (assuming it's stored in the "userId" claim)
            userId = decoded.get_payload_claim("userId").as_string();
            return true; // Token is valid
        }
        catch (const std::exception &e)
        {
            std::cerr << "Token validation failed: " << e.what() << std::endl;
            return false;
        }
    }

    std::string get_secret_key() const
    {
        const char *secret_key = std::getenv("JWT_SECRET_KEY");
        if (!secret_key)
        {
            throw std::runtime_error("JWT_SECRET_KEY is not set");
        }
        return std::string(secret_key); // Return the secret key
    }
};

#endif // AUTH_MIDDLEWARE_H
