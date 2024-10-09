#pragma once
#include <string>
#include <vector>
#include <bsoncxx/types.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>   // Required for array building
#include <bsoncxx/stdx/string_view.hpp>
// #include <bsoncxx/types.hpp>


// Define the User model class
class User {
public:
    bsoncxx::oid _id;  
    std::string username;
    std::string password;
    std::string salt;
    std::string firstName;
    std::string lastName;
    std::string bio;
    std::string email;
    std::vector<bsoncxx::document::value> bookings;  // Stores the user's bookings

    // Constructor
    User(const std::string& username, const std::string& hashed_password, const std::string& salt);

    // Function to convert the user object to BSON for MongoDB storage
    bsoncxx::document::value toBson() const;

    // Static function to create a User object from a BSON document
    static User fromBson(const bsoncxx::document::view& doc);
};

