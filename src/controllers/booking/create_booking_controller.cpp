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
#include "create_booking_controller.h"

// Function to calculate total rent
double calculateTotalRent(const std::chrono::system_clock::time_point &checkIn,
                          const std::chrono::system_clock::time_point &checkOut,
                          double price, double priceAfterTax, const Guests &guests)
{
    auto duration = std::chrono::duration_cast<std::chrono::hours>(checkOut - checkIn).count();
    int nights = duration / 24;

    double sum = (guests.adults <= 2) ? priceAfterTax : priceAfterTax + (guests.adults - 2) * (price / 2);
    sum += (guests.children > 2) ? (guests.children - 2) * (price / 4) : 0;
    sum += (guests.infants > 2) ? (guests.infants - 2) * (price / 4) : 0;

    return sum * (nights + 1);
}

void handleInitBooking(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &listingId, const std::string &userId)
{
    auto client = mongoClient.createClient();
    auto bookingsCollection = client["wanderlust2"]["bookings"];
    auto listingsCollection = client["wanderlust2"]["listings"];
    auto usersCollection = client["wanderlust2"]["users"];

    if (!bookingsCollection || !listingsCollection || !usersCollection)
    {
        res.code = 500;
        res.write("No valid collection");
        res.end();
        return;
    }

    try
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            res.code = 400;
            res.write("Invalid JSON");
            res.end();
            return;
        }

        // Parse booking details
        auto checkInStr = body["checkIn"].s();
        auto checkOutStr = body["checkOut"].s();
        Guests guests{
            static_cast<int>(body["guests"]["adults"].i()),   // Cast to avoid narrowing conversion
            static_cast<int>(body["guests"]["children"].i()), // Cast to avoid narrowing conversion
            static_cast<int>(body["guests"]["infants"].i())   // Cast to avoid narrowing conversion
        };

        std::chrono::system_clock::time_point checkIn = std::chrono::system_clock::now();  // Conversion from string needed
        std::chrono::system_clock::time_point checkOut = std::chrono::system_clock::now(); // Conversion from string needed

        // Find the listing
        bsoncxx::oid listingOid{listingId};
        bsoncxx::builder::stream::document listingFilter{};
        listingFilter << "_id" << listingOid;
        auto listingDoc = listingsCollection.find_one(listingFilter.view());
        if (!listingDoc)
        {
            res.code = 404;
            res.write("Listing not found");
            res.end();
            return;
        }
        auto listingView = listingDoc->view();
        // double price = listingView["price"].get_double();
        // double priceAfterTax = listingView["priceAfterTax"].get_double();

        auto priceElement = listingView["price"];
        double price;
        if (priceElement.type() == bsoncxx::type::k_int32)
        {
            price = static_cast<double>(priceElement.get_int32().value);
        }
        else if (priceElement.type() == bsoncxx::type::k_double)
        {
            price = priceElement.get_double().value;
        }

        auto priceAfterTaxElement = listingView["priceAfterTax"];
        double priceAfterTax;
        if (priceAfterTaxElement.type() == bsoncxx::type::k_int32)
        {
            priceAfterTax = static_cast<double>(priceAfterTaxElement.get_int32().value);
        }
        else if (priceAfterTaxElement.type() == bsoncxx::type::k_double)
        {
            priceAfterTax = priceAfterTaxElement.get_double().value;
        }

        // Calculate total rent
        double totalRent = calculateTotalRent(checkIn, checkOut, price, priceAfterTax, guests);
        // Create a new booking
        bsoncxx::builder::stream::document bookingDoc{};
        bookingDoc << "user" << bsoncxx::oid{userId}
                   << "listing" << listingOid
                   << "status" << "Booked"
                   << "checkIn" << bsoncxx::types::b_date{checkIn}
                   << "checkOut" << bsoncxx::types::b_date{checkOut}
                   << "guests" << bsoncxx::builder::stream::open_document
                   << "adults" << guests.adults
                   << "children" << guests.children
                   << "infants" << guests.infants
                   << bsoncxx::builder::stream::close_document
                   << "totalRent" << totalRent;

        auto result = bookingsCollection.insert_one(bookingDoc.view());

        // Check if the booking was inserted
        if (!result) {
            res.code = 500;
            res.write("Booking creation failed");
            res.end();
            return;
        }

        bsoncxx::oid bookingId = result->inserted_id().get_oid().value;

        // Update user bookings
        bsoncxx::builder::stream::document userFilter{};
        userFilter << "_id" << bsoncxx::oid{userId};
        bsoncxx::builder::stream::document updateUser{};
        updateUser << "$push" << bsoncxx::builder::stream::open_document << "bookings" << bookingDoc.view() << bsoncxx::builder::stream::close_document;
        // updateUser << "$push" << bsoncxx::builder::stream::open_document << "bookings" << bookingId << bsoncxx::builder::stream::close_document;
        usersCollection.update_one(userFilter.view(), updateUser.view());

        crow::json::wvalue response;
        response["bookingId"] = bookingId.to_string();
        res.code = 200;
        res.write(crow::json::dump(response));
        res.end();

        // res.code = 200;
        // res.write("Booking created successfully");
        // res.end();
    }
    catch (const std::exception &e)
    {
        res.code = 500;
        res.write("Error creating booking: ");
        res.write(e.what());
        res.end();
    }
}
