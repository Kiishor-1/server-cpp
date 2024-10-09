#include "user.h"
#include <bsoncxx/builder/stream/array.hpp>   // Required for BSON arrays
#include <bsoncxx/types/value.hpp>            // For working with BSON types

// Constructor implementation
User::User(const std::string& username, const std::string& hashed_password, const std::string& salt)
    : username(username), password(hashed_password), salt(salt) {}

// Function to convert the user object to BSON for MongoDB storage
bsoncxx::document::value User::toBson() const {
    auto doc = bsoncxx::builder::stream::document{};
    doc << "username" << username
        << "password" << password
        << "salt" << salt;

    if (!firstName.empty()) {
        doc << "firstName" << firstName;
    }

    if (!lastName.empty()) {
        doc << "lastName" << lastName;
    }

    if (!bio.empty()) {
        doc << "bio" << bio;
    }

    if (!email.empty()) {
        doc << "email" << email;
    }

    // Add bookings to the user model if available
    if (!bookings.empty()) {
        auto bookingArray = bsoncxx::builder::stream::array{};
        for (const auto& booking : bookings) {
            bookingArray << booking;
        }
        doc << "bookings" << bookingArray;
    }

    return doc << bsoncxx::builder::stream::finalize;
}

// Static function to create a User object from a BSON document
User User::fromBson(const bsoncxx::document::view& doc) {
    User user(
        std::string(doc["username"].get_string().value),  // Convert from string_view to string
        std::string(doc["password"].get_string().value),
        std::string(doc["salt"].get_string().value)
    );

    if (doc.find("_id") != doc.end()) {
        user._id = doc["_id"].get_oid().value;  // Assuming _id is stored as ObjectId
    }
    
    if (doc.find("firstName") != doc.end()) {
        user.firstName = std::string(doc["firstName"].get_string().value);
    }
    if (doc.find("lastName") != doc.end()) {
        user.lastName = std::string(doc["lastName"].get_string().value);
    }
    if (doc.find("bio") != doc.end()) {
        user.bio = std::string(doc["bio"].get_string().value);
    }
    if (doc.find("email") != doc.end()) {
        user.email = std::string(doc["email"].get_string().value);
    }

    if (doc.find("bookings") != doc.end()) {
        auto bookings = doc["bookings"].get_array().value;
        for (const auto& booking : bookings) {
            user.bookings.push_back(bsoncxx::document::value(booking.get_document().view()));
        }
    }

    return user;
}
