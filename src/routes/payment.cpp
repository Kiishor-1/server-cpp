#include "payment.h"
#include "../controllers/payment/initiate_payment_controller.h"
#include "../controllers/payment/verify_payment_controller.h"
#include "../include/middleware.h"
#include "database/mongo_client.h"


// Undefine CROW_ROUTE to avoid redefinition warning
#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
#define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

template <typename AppType>
void definePaymentRoutes(AppType& app, MongoClient& mongoClient) {
    CROW_ROUTE(app, "/payments/initiate").methods(crow::HTTPMethod::Post)([&app, &mongoClient](const crow::request& req, crow::response& res) {
        auto& auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }
        handleInitiatePayment(req, res, mongoClient, user_id);
    });

    CROW_ROUTE(app, "/payments/verify").methods(crow::HTTPMethod::Post)([&mongoClient](const crow::request& req, crow::response& res) {
        handleVerifyPayment(req, res, mongoClient);
    });
}

template void definePaymentRoutes<MyApp>(MyApp& app, MongoClient& mongoClient);
