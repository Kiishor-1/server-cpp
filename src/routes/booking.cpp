#include "booking.h"
#include "../controllers/booking/create_booking_controller.h"
#include "../controllers/booking/delete_booking_controller.h"
#include "../controllers/booking/get_user_bookings_controller.h"
#include "../include/middleware.h"
#include "database/mongo_client.h"

// Undefine CROW_ROUTE to avoid redefinition warning
#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
#define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

template <typename AppType>
void defineBookingRoutes(AppType &app, MongoClient &mongoClient)
{
    // Define the route for creating a booking
    CROW_ROUTE(app, "/bookings/<string>/create").methods(crow::HTTPMethod::Post)([&app, &mongoClient](const crow::request &req, crow::response &res, const std::string &listingId)
                                                                                 {
        auto& auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }
        handleInitBooking(req, res, mongoClient, listingId, user_id); });

    CROW_ROUTE(app, "/bookings/user").methods(crow::HTTPMethod::Get)([&app, &mongoClient](const crow::request &req, crow::response &res)
                                                                     {
        auto& auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }
        handleGetUserBookings(req, res, mongoClient, user_id); });

    // Define the route for deleting a booking
    CROW_ROUTE(app, "/bookings/<string>/delete").methods(crow::HTTPMethod::Delete)([&app, &mongoClient](const crow::request &req, crow::response &res, const std::string &listingId)
                                                                                   {
        auto& auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }
        // Ensure the parameter order matches the function declaration
        handleDeleteBooking(req, res, mongoClient, listingId, user_id); });
}

// Explicit instantiation for MyApp
template void defineBookingRoutes<MyApp>(MyApp &app, MongoClient &mongoClient);