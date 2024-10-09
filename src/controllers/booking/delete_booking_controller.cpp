#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <crow/app.h>
#include <crow/json.h>
#include <chrono>
#include "middlewares/auth_middleware.h"
#include "database/mongo_client.h"
#include "models/booking.h"
#include <iostream>
#include "delete_booking_controller.h"

// Handle deletion of booking
void handleDeleteBooking(const crow::request& req, crow::response& res, MongoClient& mongoClient, const std::string& listingId, const std::string& userId) {
    auto client = mongoClient.createClient();
    auto bookingsCollection = client["wanderlust2"]["bookings"];
    auto usersCollection = client["wanderlust2"]["users"];

    if (!bookingsCollection || !usersCollection) {
        res.code = 500;
        res.write("No valid collection");
        res.end();
        return;
    }

    try {
        // Find the booking to delete
        bsoncxx::oid listingOid{listingId};
        bsoncxx::builder::stream::document bookingFilter{};
        bookingFilter << "user" << bsoncxx::oid{userId} << "listing" << listingOid;
        auto bookingDoc = bookingsCollection.find_one(bookingFilter.view());
        if (!bookingDoc) {
            res.code = 404;
            res.write("Booking not found");
            res.end();
            return;
        }

        // Remove booking from user
        bsoncxx::builder::stream::document userFilter{};
        userFilter << "_id" << bsoncxx::oid{userId};
        bsoncxx::builder::stream::document pullBooking{};
        pullBooking << "$pull" << bsoncxx::builder::stream::open_document << "bookings" << bookingDoc->view() << bsoncxx::builder::stream::close_document;
        usersCollection.update_one(userFilter.view(), pullBooking.view());

        // Delete the booking
        bookingsCollection.delete_one(bookingFilter.view());

        res.code = 200;
        res.write("Booking deleted successfully");
        res.end();
    } catch (const std::exception& e) {
        res.code = 500;
        res.write("Error deleting booking: ");
        res.write(e.what());
        res.end();
    }
}
