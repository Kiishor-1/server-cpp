#ifndef REVIEW_H
#define REVIEW_H

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/json.h>
#include <string>

class Review {
public:
    std::string id;        // Review ID (ObjectId)
    std::string user;      // ObjectId of the user who made the review
    std::string listing;   // ObjectId of the listing being reviewed
    int rating;            // Rating given by the user (1-5)
    std::string comment;   // Text comment by the user
    std::string date;      // Date when the review was submitted

    // Constructor
    Review(const std::string &user, const std::string &listing, int rating, const std::string &comment, const std::string &date);

    // Static method to create a Review from JSON
    static Review fromJson(const crow::json::rvalue &json_data);

    // Method to convert Review to BSON
    bsoncxx::document::value toBson() const;

    // Method to validate Review fields
    void validate() const;
};

#endif // REVIEW_H
