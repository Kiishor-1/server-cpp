#include "verify_payment_controller.h"
#include <crow/json.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <openssl/hmac.h>  // OpenSSL HMAC support
#include <openssl/sha.h>    // OpenSSL SHA support
#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>  // std::setw, std::setfill

// Function to calculate HMAC SHA256
std::string calculateHMAC(const std::string &key, const std::string &data)
{
    unsigned char* digest;

    digest = HMAC(EVP_sha256(), key.c_str(), key.length(),
                  reinterpret_cast<const unsigned char *>(data.c_str()), data.length(),
                  nullptr, nullptr);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return ss.str();
}

// Function to verify Razorpay signature
bool verifyRazorpaySignature(const std::string &paymentId, const std::string &orderId, const std::string &signature)
{
    const char* razorpaySecretKey = std::getenv("RAZORPAY_KEY_SECRET");
    if (!razorpaySecretKey) {
        std::cerr << "Razorpay secret key not found in environment variables." << std::endl;
        return false; // Secret key not found
    }
    
    std::string data = orderId + "|" + paymentId;
    std::string calculatedSignature = calculateHMAC(razorpaySecretKey, data);
    return calculatedSignature == signature;
}

void handleVerifyPayment(const crow::request &req, crow::response &res, MongoClient &mongoClient)
{
    auto client = mongoClient.createClient();
    auto bookingsCollection = client["wanderlust2"]["bookings"];

    try
    {
        auto body = crow::json::load(req.body);
        std::cout << "Received body for payment verification." << std::endl;

        // Check if bookingIds are provided
        if (!body || !body.has("bookingIds") || !body.has("payload") || 
            !body["payload"].has("payment")) {
            std::cerr << "Invalid JSON: Expected bookingIds and payment structure." << std::endl;
            res.code = 400;
            res.write("Invalid JSON");
            res.end();
            return;
        }

        // Extract Razorpay payment details
        std::string paymentId = body["payload"]["payment"]["entity"]["id"].s();
        std::string orderId = body["payload"]["payment"]["entity"]["order_id"].s();
        std::string signature = body["payload"]["payment"]["entity"]["signature"].s();

        // Verify the signature
        if (!verifyRazorpaySignature(paymentId, orderId, signature))
        {
            std::cout<<paymentId<<", "<<orderId<<", "<<signature<<std::endl;
            std::cerr << "Invalid signature: " << signature << std::endl;
            res.code = 400;
            res.write("Invalid signature");
            res.end();
            return;
        }

        // Extract booking IDs from the body
        auto bookingIds = body["bookingIds"];
        if (bookingIds.t() != crow::json::type::List) {
            std::cerr << "Invalid booking IDs format." << std::endl;
            res.code = 400;
            res.write("Invalid booking IDs");
            res.end();
            return;
        }

        // Update all bookings' statuses to 'Paid' for the provided booking IDs
        for (const auto& bookingId : bookingIds)
        {
            std::string bookingIdStr = bookingId.s(); // Convert JSON string to std::string

            // Convert to BSON ObjectId
            bsoncxx::oid bookingOid{bookingIdStr}; 
            
            // Query the booking document to check the current status
            bsoncxx::builder::stream::document bookingFilter{};
            bookingFilter << "_id" << bookingOid;

            auto bookingDoc = bookingsCollection.find_one(bookingFilter.view());
            if (!bookingDoc) {
                std::cerr << "No booking found for ID: " << bookingIdStr << std::endl;
                continue;
            }

            auto bookingView = bookingDoc->view();
            std::string bookingStatus = std::string(bookingView["status"].get_string().value.data());

            // Update status to 'Paid' only if it's currently 'Booked'
            if (bookingStatus == "Booked") {
                bsoncxx::builder::stream::document updateBooking{};
                updateBooking << "$set" << bsoncxx::builder::stream::open_document
                              << "status" << "Paid"
                              << "paidAt" << bsoncxx::types::b_date{std::chrono::system_clock::now()}
                              << bsoncxx::builder::stream::close_document;

                // Perform the update operation
                auto result = bookingsCollection.update_one(bookingFilter.view(), updateBooking.view());
                if (!result) {
                    std::cerr << "Failed to update booking for ID: " << bookingIdStr << std::endl;
                }
            } else {
                std::cout << "Booking ID: " << bookingIdStr << " is not in 'Booked' status, skipping update." << std::endl;
            }
        }

        // Send success response
        res.code = 200;
        res.write("Payment verified and bookings updated to Paid (if applicable)");
        res.end();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error verifying payment: " << e.what() << std::endl; // Log the specific error
        res.code = 500;
        res.write("Error verifying payment: ");
        res.write(e.what());
        res.end();
    }
}

















// #include "verify_payment_controller.h"
// #include <crow/json.h>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <bsoncxx/types.hpp>
// #include <mongocxx/client.hpp>
// #include <mongocxx/uri.hpp>
// #include <mongocxx/exception/exception.hpp>
// #include <openssl/hmac.h>  // OpenSSL HMAC support
// #include <openssl/sha.h>    // OpenSSL SHA support
// #include <chrono>
// #include <string>
// #include <iostream>
// #include <iomanip>  // std::setw, std::setfill

// // Function to calculate HMAC SHA256
// std::string calculateHMAC(const std::string &key, const std::string &data)
// {
//     unsigned char* digest;

//     digest = HMAC(EVP_sha256(), key.c_str(), key.length(),
//                   reinterpret_cast<const unsigned char *>(data.c_str()), data.length(),
//                   nullptr, nullptr);

//     std::stringstream ss;
//     for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//     {
//         ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
//     }
//     return ss.str();
// }

// // Function to verify Razorpay signature
// bool verifyRazorpaySignature(const std::string &paymentId, const std::string &orderId, const std::string &signature)
// {
//     const char* razorpaySecretKey = std::getenv("RAZORPAY_KEY_SECRET");
//     if (!razorpaySecretKey) {
//         std::cerr << "Razorpay secret key not found in environment variables." << std::endl;
//         return false; // Secret key not found
//     }
    
//     std::string data = orderId + "|" + paymentId;
//     std::string calculatedSignature = calculateHMAC(razorpaySecretKey, data);
//     return calculatedSignature == signature;
// }

// void handleVerifyPayment(const crow::request &req, crow::response &res, MongoClient &mongoClient)
// {
//     auto client = mongoClient.createClient();
//     auto bookingsCollection = client["wanderlust2"]["bookings"];

//     try
//     {
//         auto body = crow::json::load(req.body);
//         std::cout << "Received body for payment verification." << std::endl;

//         // Check if bookingIds are provided
//         if (!body || !body.has("bookingIds") || !body.has("payload") || 
//             !body["payload"].has("payment")) {
//             std::cerr << "Invalid JSON: Expected bookingIds and payment structure." << std::endl;
//             res.code = 400;
//             res.write("Invalid JSON");
//             res.end();
//             return;
//         }

//         // Extract Razorpay payment details
//         std::string paymentId = body["payload"]["payment"]["entity"]["id"].s();
//         std::string orderId = body["payload"]["payment"]["entity"]["order_id"].s();
//         std::string signature = body["payload"]["payment"]["entity"]["signature"].s();

//         // Verify the signature
//         if (!verifyRazorpaySignature(paymentId, orderId, signature))
//         {
//             std::cout<<paymentId<<", "<<orderId<<", "<<signature<<std::endl;
//             std::cerr << "Invalid signature: " << signature << std::endl;
//             res.code = 400;
//             res.write("Invalid signature");
//             res.end();
//             return;
//         }

//         // Extract booking IDs from the body
//         auto bookingIds = body["bookingIds"];
//         if (bookingIds.t() != crow::json::type::List) {
//             std::cerr << "Invalid booking IDs format." << std::endl;
//             res.code = 400;
//             res.write("Invalid booking IDs");
//             res.end();
//             return;
//         }

//         // Update all bookings' statuses to 'Paid' for the provided booking IDs
//         for (const auto& bookingId : bookingIds)
//         {
//             std::string bookingIdStr = bookingId.s(); // Convert JSON string to std::string

//             // Convert to BSON ObjectId
//             bsoncxx::oid bookingOid{bookingIdStr}; 
//             bsoncxx::builder::stream::document updateBooking{};
//             updateBooking << "$set" << bsoncxx::builder::stream::open_document
//                           << "status" << "Paid"
//                           << "paidAt" << bsoncxx::types::b_date{std::chrono::system_clock::now()}
//                           << bsoncxx::builder::stream::close_document;

//             bsoncxx::builder::stream::document bookingFilter{};
//             bookingFilter << "_id" << bookingOid;

//             // Perform the update operation
//             auto result = bookingsCollection.update_one(bookingFilter.view(), updateBooking.view());
//             if (!result) {
//                 std::cerr << "No booking found for ID: " << bookingIdStr << std::endl;
//             }
//         }

//         // Send success response
//         res.code = 200;
//         res.write("Payment verified and bookings updated to Paid");
//         res.end();
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error verifying payment: " << e.what() << std::endl; // Log the specific error
//         res.code = 500;
//         res.write("Error verifying payment: ");
//         res.write(e.what());
//         res.end();
//     }
// }

