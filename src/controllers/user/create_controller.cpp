#include "create_controller.h"

void handleCreateUser(const crow::request& req, crow::response& res) {
    // Example JSON response
    crow::json::wvalue response;
    response["message"] = "User created";
    res.set_header("Content-Type", "application/json");
    res.write(crow::json::dump(response));
    res.end();
}
