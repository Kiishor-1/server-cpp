#include "signup.h"
#include "password_utils.h"
#include "crow.h"
#include "database/mongo_client.h"
#include "models/user.h"  // Include the User model
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>

void handleUserRegistration(const crow::request &req, crow::response &res, MongoClient &mongoClient) {
    auto client = mongoClient.createClient();
    auto collection = client["wanderlust2"]["users"];

    if (!collection) {
        std::cerr << "No valid collection" << std::endl;
        res.code = 500;
        res.write("No valid collection");
        res.end();
        return;
    }

    try {
        // Parse the JSON body of the request
        auto json_body = crow::json::load(req.body);
        if (!json_body) {
            std::cerr << "Invalid JSON" << std::endl;
            res.code = 400;
            res.write("Invalid JSON");
            res.end();
            return;
        }

        // Fetch required fields
        std::string username = json_body["username"].s();
        std::string password = json_body["password"].s();
        std::string confirmPassword = json_body["confirmPassword"].s();
        std::string firstName = json_body["firstName"].s();
        std::string lastName = json_body["lastName"].s();
        std::string email = json_body["email"].s();
        std::string bio = json_body["bio"].s();

        // Check if password and confirmPassword match
        if (password != confirmPassword) {
            std::cerr << "Passwords do not match" << std::endl;
            res.code = 400;
            res.write("Passwords do not match.");
            res.end();
            return;
        }

        // Check if the user already exists
        auto existing_user = collection.find_one(bsoncxx::builder::stream::document{} << "username" << username << bsoncxx::builder::stream::finalize);
        if (existing_user) {
            std::cerr << "User already exists" << std::endl;
            res.code = 400;
            res.write("User already exists.");
            res.end();
            return;
        }

        // Generate salt and hash the password
        std::string salt = generateSalt();
        std::string hashed_password = hashPassword(password, salt);

        // Create a new User instance
        User newUser(username, hashed_password, salt);

        // Set additional fields in the User object
        newUser.firstName = firstName;
        newUser.lastName = lastName;
        newUser.email = email;
        newUser.bio = bio;

        // Insert into the database
        collection.insert_one(newUser.toBson().view());

        res.code = 200;
        res.write("User registered successfully.");
        res.end();
    }
    catch (const bsoncxx::exception &e) {
        std::cerr << "BSON error: " << e.what() << std::endl;
        res.code = 500;
        res.write("BSON format error");
        res.end();
    }
    catch (const mongocxx::exception &e) {
        std::cerr << "MongoDB error: " << e.what() << std::endl;
        res.code = 500;
        res.write("Database error");
        res.end();
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        res.code = 500;
        res.write("Internal server error");
        res.end();
    }
}
