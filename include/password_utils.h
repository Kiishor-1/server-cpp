#ifndef PASSWORD_UTILS_H
#define PASSWORD_UTILS_H

#include <string>

// Function declarations
std::string hashPassword(const std::string &password, const std::string &salt);
std::string generateSalt();

#endif // PASSWORD_UTILS_H
