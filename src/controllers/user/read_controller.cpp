#include "read_controller.h"

void handleReadUser(const crow::request& req, crow::response& res) {
    // Example JSON response
    crow::json::wvalue response;
    response["message"] = "User details";
    res.set_header("Content-Type", "application/json");
    res.write(crow::json::dump(response));
    res.end();
}
