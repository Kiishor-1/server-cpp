#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <chrono>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>

struct Guests {
    int adults;
    int children;
    int infants;
};

struct Booking {
    bsoncxx::oid user;
    bsoncxx::oid listing;
    std::string status;
    std::chrono::system_clock::time_point checkIn;
    std::chrono::system_clock::time_point checkOut;
    Guests guests;
    double totalRent;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point paidAt;
};

// Function declarations
void to_bson(bsoncxx::builder::stream::document& doc, const Booking& booking);
void from_bson(const bsoncxx::document::view& view, Booking& booking);

#endif // BOOKING_H



