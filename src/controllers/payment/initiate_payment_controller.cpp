#include <string>
#include "initiate_payment_controller.h"
#include <curl/curl.h>  // Include curl header
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/array.hpp> // Include for basic array builder
#include "dotenv.h"

// Callback function to handle the response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void handleInitiatePayment(const crow::request &req, crow::response &res, MongoClient &mongoClient, const std::string &user_id)
{
    auto client = mongoClient.createClient();
    auto bookingsCollection = client["wanderlust2"]["bookings"];

    try
    {
        // Load the request body as JSON
        auto body = crow::json::load(req.body);
        if (!body || !body.has("bookingIds") || body["bookingIds"].t() != crow::json::type::List)
        {
            res.code = 400;
            res.write("Invalid JSON or bookingIds not found or is not an array");
            res.end();
            return;
        }

        auto bookingIds = body["bookingIds"];

        // List to store the booking OIDs
        std::vector<bsoncxx::oid> bookingOids;

        double totalRent = 0.0; // Variable to accumulate the total rent for all bookings

        // Loop through all booking IDs and fetch corresponding bookings from MongoDB
        for (const auto& bookingId : bookingIds)
        {
            std::string bookingIdStr = bookingId.s();  // Convert JSON string to std::string

            // Convert the booking ID string into MongoDB ObjectId
            bsoncxx::oid bookingOid{bookingIdStr};
            bookingOids.push_back(bookingOid);

            // Query MongoDB for the booking document
            bsoncxx::builder::stream::document bookingFilter{};
            bookingFilter << "_id" << bookingOid;

            auto bookingDoc = bookingsCollection.find_one(bookingFilter.view());
            if (!bookingDoc)
            {
                res.code = 404;
                res.write("Booking not found for ID: " + bookingIdStr);
                res.end();
                return;
            }

            // Add the rent from this booking to the total rent
            auto bookingView = bookingDoc->view();

             // Check if the booking belongs to the logged-in user
            if (bookingView["user"].get_oid().value.to_string() != user_id) {
                res.code = 403;  // Forbidden
                res.write("You do not have permission to initiate payment for booking ID: " + bookingIdStr);
                res.end();
                return;
            }

            // Check if the booking is already paid
            std::string bookingStatus = bookingView["status"].get_string().value.data();
            if (bookingStatus != "Booked" || bookingStatus == "Paid")
            {
                res.code = 400;
                res.write("Booking ID " + bookingIdStr + " is already paid or not in 'Booked' status.");
                res.end();
                return;
            }

            
            totalRent += bookingView["totalRent"].get_double();  // Assuming "totalRent" is a double field
        }

        // After fetching all bookings and calculating the total rent, initiate the Razorpay payment
        const char* keyId = std::getenv("RAZORPAY_KEY_ID");
        const char* keySecret = std::getenv("RAZORPAY_KEY_SECRET");

        if (!keyId || !keySecret) {
            res.code = 500;
            res.write("Razorpay credentials not found in environment variables.");
            res.end();
            return;
        }

        // Prepare Razorpay order details
        crow::json::wvalue orderPayload;
        orderPayload["amount"] = static_cast<int>(totalRent * 100); // Ensure this is an integer
        orderPayload["currency"] = "INR";

        // Serialize ObjectIds for the receipt
        bsoncxx::builder::basic::array receiptArrayBuilder;
        for (const auto& oid : bookingOids)
        {
            receiptArrayBuilder.append(oid.to_string());
        }

        // Ensure receipt is correctly formed
        orderPayload["receipt"] = bsoncxx::to_json(receiptArrayBuilder.view()); // Ensure this is a string or valid JSON
        orderPayload["payment_capture"] = 1; // Auto capture

        // Log the order payload for debugging
        std::cout << "Order Payload: " << crow::json::dump(orderPayload) << std::endl;

        // Initialize curl
        CURL* curl;
        CURLcode resCurl;
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if(curl) {
            // Set the URL for the request
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.razorpay.com/v1/orders");
            curl_easy_setopt(curl, CURLOPT_POST, 1L);  // Set to POST request

            // Set the headers
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            // Set the authentication
            std::string userpwd = std::string(keyId) + ":" + std::string(keySecret);
            curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.c_str());

            // Prepare JSON payload
            std::string jsonPayload = crow::json::dump(orderPayload);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());

            // Set the write callback function
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);  // Set the headers

            // Perform the request
            resCurl = curl_easy_perform(curl);

            // Check for errors
            if (resCurl != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(resCurl) << std::endl;
                res.code = 500;
                res.write("Failed to create Razorpay order.");
                res.end();
                return;
            }

            // Cleanup headers
            curl_slist_free_all(headers);

            // Parse Razorpay response
            auto orderResponseJson = crow::json::load(readBuffer);
            std::string orderId = orderResponseJson["id"].s(); // Razorpay order ID

            // Prepare the response to send to the frontend
            crow::json::wvalue response;
            response["orderId"] = orderId;
            response["amount"] = static_cast<int>(totalRent * 100);
            response["currency"] = "INR";
            res.write(crow::json::dump(response));
            res.end();

            // Cleanup curl
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
    }
    catch (const std::exception &e)
    {
        res.code = 500;
        res.write("Error initiating payment: " + std::string(e.what()));
        res.end();
    }
}
