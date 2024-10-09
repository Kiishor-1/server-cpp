#include "jwt_utils.h"
#include <openssl/ssl.h>
#include <iostream>
#include "dotenv.h"
#include <cstdlib>  // For getenv

std::string getJWTSecret() {
    const char* secret = std::getenv("JWT_SECRET_KEY");
    if (!secret) {
        throw std::runtime_error("JWT_SECRET_KEY environment variable is not set");
    }
    return std::string(secret);  // Convert to std::string
}

std::string JWTUtils::generateToken(const std::string& userId) {
    auto token = jwt::create()
        .set_issuer("auth-server")
        .set_type("JWS")
        .set_payload_claim("userId", jwt::claim(userId))
        .sign(jwt::algorithm::hs256{getJWTSecret()});  // Fetch JWT secret at runtime
    return token;
}

bool JWTUtils::verifyToken(const std::string& token, std::string& userId) {
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
                            .allow_algorithm(jwt::algorithm::hs256{getJWTSecret()})  // Fetch JWT secret at runtime
                            .with_issuer("auth-server");

        verifier.verify(decoded);

        userId = decoded.get_payload_claim("userId").as_string();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

