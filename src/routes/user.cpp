#include "user.h"
#include "../controllers/user/create_controller.h"
#include "../controllers/user/read_controller.h"
#include "../../include/middleware.h"
#include "../../include/middlewares/is_logged_in.h"

// Undefine CROW_ROUTE to avoid redefinition warning
#ifdef CROW_ROUTE
#undef CROW_ROUTE
#endif

#define CROW_ROUTE(app, url) app.template route<crow::black_magic::get_parameter_tag(url)>(url)
#define CROW_MAKE_HANDLER(route, handler) route.handler(handler)

template <typename AppType>
void defineUserRoutes(AppType &app)
{
    isLoggedIn loginMiddleware;
    typename isLoggedIn::context ctx; // Instantiate context

    // Define route for creating a user
    CROW_ROUTE(app, "/users/create")
        .methods(crow::HTTPMethod::Post)([&app, &loginMiddleware, &ctx](const crow::request &req, crow::response &res)
                                         { 
                                             loginMiddleware.before_handle(req, res, ctx);
                                             if (res.code != 200) return;
                                             handleCreateUser(req, res); 
                                             loginMiddleware.after_handle(req, res, ctx); });

    // Define route for reading a user (with isLoggedIn middleware)
    CROW_ROUTE(app, "/users/read")
        .methods(crow::HTTPMethod::Get)([&app, &loginMiddleware, &ctx](const crow::request &req, crow::response &res)
                                        {
                                            loginMiddleware.before_handle(req, res, ctx);
                                            if (res.code != 200) return;
                                            handleReadUser(req, res); 
                                            loginMiddleware.after_handle(req, res, ctx); });
}

// Explicit template instantiation for the expected App type
template void defineUserRoutes<MyApp>(MyApp &app);

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
