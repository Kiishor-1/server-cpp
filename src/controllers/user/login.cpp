#include "login.h"
#include "password_utils.h"  
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/exception/exception.hpp>
#include "jwt_utils.h"
#include "models/user.h"  // Include the User model
#include <iostream>

void handleUserLogin(const crow::request &req, crow::response &res, MongoClient &mongoClient) {
    auto client = mongoClient.createClient();
    auto collection = client["wanderlust2"]["users"];

    auto json_body = crow::json::load(req.body);
    if (!json_body) {
        res.code = 400;
        res.write("Invalid JSON");
        res.end();
        return;
    }

    std::string username = json_body["username"].s();
    std::string password = json_body["password"].s();

    try {
        auto userDoc = collection.find_one(bsoncxx::builder::stream::document{} << "username" << username << bsoncxx::builder::stream::finalize);
        if (!userDoc) {
            res.code = 404;
            res.write("User not found.");
            res.end();
            return;
        }

        // Deserialize user document to User object
        User user = User::fromBson(userDoc->view());

        // Hash the input password with the stored salt and compare
        std::string hashed_input_password = hashPassword(password, user.salt);
        if (hashed_input_password != user.password) {
            res.code = 401;  
            res.write("Invalid password.");
            res.end();
            return;
        }

        // Generate JWT token with userId
        std::string token = JWTUtils::generateToken(user._id.to_string());

        // Construct the response JSON
        crow::json::wvalue user_response;
        user_response["token"] = token;
        user_response["user"]["username"] = user.username;
        user_response["user"]["userId"] = user._id.to_string();

        std::string str = user._id.to_string();
        std::cout<<str<<std::endl;

        res.code = 200;  
        res.set_header("Content-Type", "application/json");
        res.write(crow::json::dump(user_response));  
    }
    catch (const mongocxx::exception &e) {
        res.code = 500;
        res.write("Failed to login: ");
        res.write(e.what());
    }
    res.end();
}
