#include "user.h"
#include "controllers/user/signup.h"
#include "controllers/user/login.h"
#include "controllers/user/user_profile.h"
#include "../../include/middleware.h"
#include "database/mongo_client.h"
#include "crow.h"

// Undefine CROW_ROUTE to avoid redefinition warning
#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
#define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

template <class AppType>
void defineUserRoutes(AppType& app, MongoClient& mongoClient) {
    // Define user-related routes using `app` and `mongoClient`
    
    // Use the login handler
    CROW_ROUTE(app, "/user/login").methods(crow::HTTPMethod::Post)
    ([&mongoClient](const crow::request& req, crow::response& res) {
        handleUserLogin(req, res, mongoClient);
    });

    // Use the registration handler
    CROW_ROUTE(app, "/user/register").methods(crow::HTTPMethod::Post)
    ([&mongoClient](const crow::request& req, crow::response& res) {
        handleUserRegistration(req, res, mongoClient);
    });

    CROW_ROUTE(app, "/user/profile").methods(crow::HTTPMethod::Get)
    ([&app, &mongoClient](const crow::request &req, crow::response &res) {
        auto &auth_context = app.template get_context<AuthMiddleware>(req);
        std::string user_id = auth_context.user_id;

        if (user_id.empty()) {
            res.code = 403;
            res.write("Unauthorized access: user_id missing.");
            res.end();
            return;
        }

        handleGetUserProfile(req, res, mongoClient, user_id);
    });
}

// Explicitly instantiate the template for your application
template void defineUserRoutes<MyApp>(MyApp& app, MongoClient& mongoClient);

















// #include "user.h"
// #include "controllers/user/signup.h"
// #include "controllers/user/login.h"
// #include "../../include/middleware.h"
// #include "database/mongo_client.h"
// #include "crow.h"

// // Undefine CROW_ROUTE to avoid redefinition warning
// #ifdef CROW_ROUTE
// #undef CROW_ROUTE
// #endif

// #define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
// #define CROW_MAKE_HANDLER(route, handler) route.handler(handler)


// template <class AppType>
// void defineUserRoutes(AppType& app, MongoClient& mongoClient) {
//     // Define user-related routes using `app` and `mongoClient`
//     CROW_ROUTE(app, "/user/login")
//     ([&mongoClient](const crow::request& req) {
//         // Implement login logic
//         return crow::response(200, "User login endpoint");
//     });

//     CROW_ROUTE(app, "/user/register")
//     ([&mongoClient](const crow::request& req) {
//         // Implement register logic
//         return crow::response(200, "User registration endpoint");
//     });
// }

// // Explicitly instantiate the template for your application
// template void defineUserRoutes<MyApp>(MyApp& app, MongoClient& mongoClient);





















// #include "user.h"
// #include "../controllers/user/create_controller.h"
// #include "../controllers/user/read_controller.h"
// #include "../../include/middleware.h"
// #include "../../include/middlewares/is_logged_in.h"

// // Undefine CROW_ROUTE to avoid redefinition warning
// #ifdef CROW_ROUTE
// #undef CROW_ROUTE
// #endif

// #define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
// #define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

// template <typename AppType>
// void defineUserRoutes(AppType &app)
// {
//     isLoggedIn loginMiddleware;
//     typename isLoggedIn::context ctx; // Instantiate context

//     // Define route for creating a user
//     CROW_ROUTE(app, "/users/create")
//         .methods(crow::HTTPMethod::Post)([&app, &loginMiddleware, &ctx](const crow::request &req, crow::response &res)
//                                          { 
//                                              loginMiddleware.before_handle(req, res, ctx);
//                                              if (res.code != 200) return;
//                                              handleCreateUser(req, res); 
//                                              loginMiddleware.after_handle(req, res, ctx); });

//     // Define route for reading a user (with isLoggedIn middleware)
//     CROW_ROUTE(app, "/users/read")
//         .methods(crow::HTTPMethod::Get)([&app, &loginMiddleware, &ctx](const crow::request &req, crow::response &res)
//                                         {
//                                             loginMiddleware.before_handle(req, res, ctx);
//                                             if (res.code != 200) return;
//                                             handleReadUser(req, res); 
//                                             loginMiddleware.after_handle(req, res, ctx); });
// }

// // Explicit template instantiation for the expected App type
// template void defineUserRoutes<MyApp>(MyApp &app);

// #include "user.h"
//  #include "../controllers/user/create_controller.h"
//  #include "../controllers/user/read_controller.h"
//  #include "../../include/middleware.h" // Adjust the path as needed

// // Undefine CROW_ROUTE to avoid redefinition warning
// #ifdef CROW_ROUTE
// #undef CROW_ROUTE
// #endif

// #define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
// #define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

// template <typename AppType>
// void defineUserRoutes(AppType &app)
// {
//     // Define route for creating a user
//     CROW_ROUTE(app, "/users/create")
//         .methods(crow::HTTPMethod::Post)([](const crow::request &req, crow::response &res)
//                                          { handleCreateUser(req, res); });

//     // Define route for reading a user
//     CROW_ROUTE(app, "/users/read")
//         .methods(crow::HTTPMethod::Get)([](const crow::request &req, crow::response &res)
//                                         { handleReadUser(req, res); });
// }

// // Explicit template instantiation for the expected App type
// // template void defineUserRoutes<crow::App<CORS,LogMiddleware>>(crow::App<CORS,LogMiddleware> &app);
// // template void defineUserRoutes<CrowApp<CORS, LogMiddleware>>(CrowApp<CORS, LogMiddleware>& app);
// template void defineUserRoutes<MyApp>(MyApp &app);
