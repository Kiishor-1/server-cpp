#ifndef JWT_UTILS_H
#define JWT_UTILS_H

#include <string>
#include <jwt-cpp/jwt.h>

class JWTUtils {
public:
    static std::string generateToken(const std::string& userId);
    static bool verifyToken(const std::string& token, std::string& userId);
};

#endif // JWT_UTILS_H
