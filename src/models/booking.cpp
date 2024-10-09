#include "booking.h"
#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <chrono>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>

// Converts a std::chrono::system_clock::time_point to a bsoncxx::types::b_date
bsoncxx::types::b_date chrono_to_bson_date(const std::chrono::system_clock::time_point& tp) {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    return bsoncxx::types::b_date{std::chrono::milliseconds{ms.count()}};
}

// Converts a bsoncxx::types::b_date to a std::chrono::system_clock::time_point
std::chrono::system_clock::time_point bson_date_to_chrono(const bsoncxx::types::b_date& bdate) {
    auto ms = bdate.to_int64();
    return std::chrono::system_clock::time_point{std::chrono::milliseconds{ms}};
}

// Converts a Booking object to BSON document
void to_bson(bsoncxx::builder::stream::document& doc, const Booking& booking) {
    doc << "user" << booking.user
        << "listing" << booking.listing
        << "status" << booking.status
        << "checkIn" << chrono_to_bson_date(booking.checkIn)
        << "checkOut" << chrono_to_bson_date(booking.checkOut)
        << "guests" << bsoncxx::builder::stream::open_document
            << "adults" << booking.guests.adults
            << "children" << booking.guests.children
            << "infants" << booking.guests.infants
        << bsoncxx::builder::stream::close_document
        << "totalRent" << booking.totalRent
        << "createdAt" << chrono_to_bson_date(booking.createdAt)
        << "paidAt" << chrono_to_bson_date(booking.paidAt);
}

// Converts a BSON document view to a Booking object
void from_bson(const bsoncxx::document::view& view, Booking& booking) {
    booking.user = view["user"].get_oid().value;
    booking.listing = view["listing"].get_oid().value;
    booking.status = std::string(view["status"].get_string().value.data());

    booking.checkIn = bson_date_to_chrono(view["checkIn"].get_date());
    booking.checkOut = bson_date_to_chrono(view["checkOut"].get_date());

    auto guestsDoc = view["guests"].get_document().view();
    booking.guests.adults = guestsDoc["adults"].get_int32();
    booking.guests.children = guestsDoc["children"].get_int32();
    booking.guests.infants = guestsDoc["infants"].get_int32();

    booking.totalRent = view["totalRent"].get_double().value;

    booking.createdAt = bson_date_to_chrono(view["createdAt"].get_date());
    booking.paidAt = bson_date_to_chrono(view["paidAt"].get_date());
}


