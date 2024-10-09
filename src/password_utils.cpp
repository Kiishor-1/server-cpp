#include <openssl/evp.h>
#include <openssl/rand.h>
#include "password_utils.h"

const int SALT_SIZE = 16;        // Salt size (in bytes)
const int HASH_SIZE = 32;        // Hash size (256-bit hash)
const int ITERATIONS = 10000;    // PBKDF2 iteration count

// Function to generate random salt
std::string generateSalt() {
    unsigned char salt[SALT_SIZE];
    RAND_bytes(salt, sizeof(salt)); // Generate a random salt

    return std::string(reinterpret_cast<const char*>(salt), SALT_SIZE);
}

// Function to hash a password using PBKDF2
std::string hashPassword(const std::string &password, const std::string &salt) {
    unsigned char hash[HASH_SIZE];

    PKCS5_PBKDF2_HMAC(
        password.c_str(), password.size(),
        reinterpret_cast<const unsigned char*>(salt.c_str()), salt.size(),
        ITERATIONS, EVP_sha256(), HASH_SIZE, hash
    );

    return std::string(reinterpret_cast<const char*>(hash), HASH_SIZE);
}
