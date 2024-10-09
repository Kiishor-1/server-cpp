#include "review.h"

// Constructor
Review::Review(const std::string &user, const std::string &listing, int rating, const std::string &comment, const std::string &date)
    : user(user), listing(listing), rating(rating), comment(comment), date(date) {}

// Validate Review fields
void Review::validate() const {
    if (rating < 1 || rating > 5) throw std::invalid_argument("Rating must be between 1 and 5");
    if (comment.empty()) throw std::invalid_argument("Comment cannot be empty");
}

// Convert to BSON for MongoDB
bsoncxx::document::value Review::toBson() const {
    bsoncxx::builder::stream::document doc{};
    doc << "user" << bsoncxx::oid{user}  // Convert user ID to OID
        << "listing" << bsoncxx::oid{listing}  // Convert listing ID to OID
        << "rating" << rating
        << "comment" << comment
        << "date" << date;
    return doc << bsoncxx::builder::stream::finalize;
}

// Static method to create a Review object from JSON (excluding user and listing)
Review Review::fromJson(const crow::json::rvalue &json_data) {
    return Review(
        "",  // User will be set in the handler
        "",  // Listing will be set in the handler
        json_data["rating"].i(),
        json_data["comment"].s(),
        json_data.has("date") ? json_data["date"].s() : std::string("")  // Ensure both return types are std::string
    );
}















// #include "review.h"

// // Constructor
// Review::Review(const std::string &user, const std::string &listing, int rating, const std::string &comment, const std::string &date)
//     : user(user), listing(listing), rating(rating), comment(comment), date(date) {}

// // Validate Review fields
// void Review::validate() const {
//     if (rating < 1 || rating > 5) throw std::invalid_argument("Rating must be between 1 and 5");
//     if (comment.empty()) throw std::invalid_argument("Comment cannot be empty");
// }

// // Convert to BSON for MongoDB
// bsoncxx::document::value Review::toBson() const {
//     bsoncxx::builder::stream::document doc{};
//     doc << "user" << bsoncxx::oid{user}
//         << "listing" << bsoncxx::oid{listing}
//         << "rating" << rating
//         << "comment" << comment
//         << "date" << date;
//     return doc << bsoncxx::builder::stream::finalize;
// }

// // Static method to create a Review object from JSON
// Review Review::fromJson(const crow::json::rvalue &json_data) {
//     return Review(
//         json_data["user"].s(),
//         json_data["listing"].s(),
//         json_data["rating"].i(),
//         json_data["comment"].s(),
//         json_data["date"].s()
//     );
// }
