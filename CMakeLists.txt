# cmake_minimum_required(VERSION 3.10)
# project(CrowServer)

# # Set C++ standard
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Boost settings for Docker environment
# set(BOOST_ROOT "/usr/include")        # Boost headers in Docker
# set(BOOST_LIBRARYDIR "/usr/lib")      # Boost library directory in Docker
# include_directories(${BOOST_ROOT})
# link_directories(${BOOST_LIBRARYDIR})

# # Specify Boost libraries (using system, thread, and filesystem)
# set(Boost_LIBRARIES
#     boost_system
#     boost_thread
#     boost_filesystem
# )

# # MongoDB C++ Driver paths (assuming installation in Docker at /usr/local)
# include_directories("/usr/local/include/mongocxx/v_noabi" "/usr/local/include/bsoncxx/v_noabi")
# link_directories("/usr/local/lib")

# # Crow framework include path (included from project source)
# set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# # JWT-CPP Library (assuming it is also installed in /usr/local)
# set(JWT_CPP_INCLUDE_DIR "/usr/local/include/jwt-cpp")
# include_directories(${JWT_CPP_INCLUDE_DIR})

# # OpenSSL and cURL paths manually set (avoid find_package)
# set(OPENSSL_INCLUDE_DIR "/usr/include/openssl")          # OpenSSL include path for Docker
# set(OPENSSL_CRYPTO_LIBRARY "/usr/lib/x86_64-linux-gnu/libcrypto.so")  # OpenSSL Crypto library path for Docker
# set(OPENSSL_SSL_LIBRARY "/usr/lib/x86_64-linux-gnu/libssl.so")        # OpenSSL SSL library path for Docker

# set(CURL_INCLUDE_DIR "/usr/include/curl")               # cURL include path for Docker
# set(CURL_LIBRARY "/usr/lib/x86_64-linux-gnu/libcurl.so") # cURL library path for Docker

# # Include directories for project source and headers
# include_directories(
#     ${CROW_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/src
#     ${OPENSSL_INCLUDE_DIR}  # Added OpenSSL include directory
#     ${CURL_INCLUDE_DIR}     # Added cURL include directory
# )

# # Specify source files for the project
# set(SOURCE_FILES
#     ${CMAKE_SOURCE_DIR}/src/main.cpp
#     ${CMAKE_SOURCE_DIR}/src/jwt_utils.cpp
#     ${CMAKE_SOURCE_DIR}/src/routes/listings.cpp
#     ${CMAKE_SOURCE_DIR}/src/routes/user.cpp
#     ${CMAKE_SOURCE_DIR}/src/routes/reviews.cpp
#     ${CMAKE_SOURCE_DIR}/src/routes/booking.cpp         # Booking route added
#     ${CMAKE_SOURCE_DIR}/src/routes/payment.cpp         # Payment route added
#     ${CMAKE_SOURCE_DIR}/src/models/listing.cpp
#     ${CMAKE_SOURCE_DIR}/src/models/review.cpp
#     ${CMAKE_SOURCE_DIR}/src/models/user.cpp
#     ${CMAKE_SOURCE_DIR}/src/models/booking.cpp         # Booking model added
#     ${CMAKE_SOURCE_DIR}/src/models/payment.cpp         # Payment model added
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/create_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/delete_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/read_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/read_one_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/update_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/booking/create_booking_controller.cpp  # Added create booking controller
#     ${CMAKE_SOURCE_DIR}/src/controllers/booking/delete_booking_controller.cpp  # Added delete booking controller
#     ${CMAKE_SOURCE_DIR}/src/controllers/booking/get_user_bookings_controller.cpp  # Added get user bookings controller
#     ${CMAKE_SOURCE_DIR}/src/controllers/payment/initiate_payment_controller.cpp  # Added initiate payment controller
#     ${CMAKE_SOURCE_DIR}/src/controllers/payment/verify_payment_controller.cpp    # Added verify payment controller
#     ${CMAKE_SOURCE_DIR}/src/controllers/user/create_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/user/login.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/user/signup.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/user/user_profile.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/reviews/create_review_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/reviews/read_reviews_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/reviews/delete_review_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/database/mongo_client.cpp
#     ${CMAKE_SOURCE_DIR}/src/password_utils.cpp        # Password utilities
#     ${CMAKE_SOURCE_DIR}/include/middleware.h
# )

# # Add executable for the CrowServer
# add_executable(CrowServer ${SOURCE_FILES})


# # Find and link required packages
# find_package(Threads REQUIRED)
# find_package(Boost REQUIRED COMPONENTS system thread filesystem)
# find_package(OpenSSL REQUIRED)      # Ensure OpenSSL for JWT-CPP
# find_package(CURL REQUIRED)   

# # Link libraries (Boost, MongoDB, Threads, OpenSSL, and cURL)
# target_link_libraries(CrowServer
#     ${Boost_LIBRARIES}
#     mongocxx
#     bsoncxx
#     Threads::Threads
#     ${OPENSSL_CRYPTO_LIBRARY}   # OpenSSL Crypto library linking
#     ${OPENSSL_SSL_LIBRARY}      # OpenSSL SSL library linking
#     ${CURL_LIBRARY}             # cURL library linking
#     # ws2_32                      # Winsock for Windows networking (if needed)
# )




cmake_minimum_required(VERSION 3.10)
project(CrowServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths to Boost libraries
set(BOOST_ROOT "C:/local/boost_1_85_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_85_0/stage/lib")
include_directories("${BOOST_ROOT}")
link_directories("${BOOST_LIBRARYDIR}")

# Paths to MongoDB C++ driver
set(MONGO_CXX_DRIVER_ROOT "C:/mongo-cxx-driver")
set(MONGO_CXX_DRIVER_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include")
set(LIBBSONCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/bsoncxx/v_noabi")
set(LIBMONGOCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/mongocxx/v_noabi")
set(LIBMONGOCXX_LIB_DIR "${MONGO_CXX_DRIVER_ROOT}/lib")

# Path to Crow framework
set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# Path to JWT-CPP library
set(JWT_CPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/jwt-cpp")

# Path to OpenSSL in MSYS2 for cross-platform cryptographic functions
set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.a")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.a")

# Path to Curl library
set(CURL_INCLUDE_DIR "C:/msys64/mingw64/include/curl")
set(CURL_LIBRARY "C:/msys64/mingw64/lib/libcurl.dll.a")


# Include directories
include_directories(
    ${CROW_INCLUDE_DIR}
    ${MONGO_CXX_DRIVER_INCLUDE_DIR}
    ${LIBBSONCXX_INCLUDE_DIR}
    ${LIBMONGOCXX_INCLUDE_DIR}
    ${JWT_CPP_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${OPENSSL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
)

# Boost libraries
set(Boost_LIBRARIES 
    boost_system-mgw14-mt-x64-1_85 
    boost_thread-mgw14-mt-x64-1_85 
    boost_filesystem-mgw14-mt-x64-1_85)

# MongoDB libraries
set(MONGOCXX_LIBRARIES 
    "${LIBMONGOCXX_LIB_DIR}/libmongocxx.dll.a"
    "${LIBMONGOCXX_LIB_DIR}/libbsoncxx.dll.a")

# Find Threads package
find_package(Threads REQUIRED)

# Specify source files
set(SOURCE_FILES
${CMAKE_SOURCE_DIR}/src/main.cpp
${CMAKE_SOURCE_DIR}/src/jwt_utils.cpp
${CMAKE_SOURCE_DIR}/src/routes/listings.cpp
${CMAKE_SOURCE_DIR}/src/routes/user.cpp
${CMAKE_SOURCE_DIR}/src/routes/reviews.cpp
${CMAKE_SOURCE_DIR}/src/routes/booking.cpp  # Added booking route
${CMAKE_SOURCE_DIR}/src/routes/payment.cpp  # Added payment route
${CMAKE_SOURCE_DIR}/src/models/listing.cpp
${CMAKE_SOURCE_DIR}/src/models/review.cpp
${CMAKE_SOURCE_DIR}/src/models/user.cpp
${CMAKE_SOURCE_DIR}/src/models/booking.cpp  # Added booking model
${CMAKE_SOURCE_DIR}/src/models/payment.cpp  # Added payment model
${CMAKE_SOURCE_DIR}/src/controllers/listings/create_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/listings/delete_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/listings/read_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/listings/read_one_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/listings/update_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/booking/create_booking_controller.cpp  # Added create booking controller
${CMAKE_SOURCE_DIR}/src/controllers/booking/delete_booking_controller.cpp  # Added delete booking controller
${CMAKE_SOURCE_DIR}/src/controllers/booking/get_user_bookings_controller.cpp  # Added get user bookings controller
${CMAKE_SOURCE_DIR}/src/controllers/payment/initiate_payment_controller.cpp  # Added initiate payment controller
${CMAKE_SOURCE_DIR}/src/controllers/payment/verify_payment_controller.cpp  # Added verify payment controller
${CMAKE_SOURCE_DIR}/src/controllers/user/create_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/user/login.cpp
${CMAKE_SOURCE_DIR}/src/controllers/user/signup.cpp
${CMAKE_SOURCE_DIR}/src/controllers/user/user_profile.cpp
${CMAKE_SOURCE_DIR}/src/controllers/reviews/create_review_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/reviews/read_reviews_controller.cpp
${CMAKE_SOURCE_DIR}/src/controllers/reviews/delete_review_controller.cpp
${CMAKE_SOURCE_DIR}/src/database/mongo_client.cpp
${CMAKE_SOURCE_DIR}/src/password_utils.cpp  # Shared password utility file
${CMAKE_SOURCE_DIR}/include/middleware.h
)

# Add executable
add_executable(CrowServer ${SOURCE_FILES})

# Include Winsock library (for networking on Windows)
if(WIN32)
    target_link_libraries(CrowServer ws2_32 crypt32 mswsock)  # Link Crypt32 for OpenSSL on Windows
else()
    target_link_libraries(CrowServer Threads::Threads ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
endif()

# Link libraries
target_link_libraries(CrowServer
    ${Boost_LIBRARIES}
    ${MONGOCXX_LIBRARIES}
    Threads::Threads
    ${OPENSSL_CRYPTO_LIBRARY}
    ${OPENSSL_SSL_LIBRARY}
    ${CURL_LIBRARY}
    ws2_32  # Winsock for Windows
)







# cmake_minimum_required(VERSION 3.10)
# project(CrowServer)

# # Set C++ standard
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Explicitly set Boost paths (adjust the paths if necessary)
# set(BOOST_ROOT "/usr/include")
# set(BOOST_LIBRARYDIR "/usr/lib")

# # Manually include Boost directories
# include_directories(${BOOST_ROOT})
# link_directories(${BOOST_LIBRARYDIR})

# # Specify Boost libraries
# set(Boost_LIBRARIES
#     boost_system
#     boost_thread
#     boost_filesystem
# )

# # MongoDB C++ driver paths (assuming it's installed in Docker)
# include_directories("/usr/local/include/mongocxx/v_noabi" "/usr/local/include/bsoncxx/v_noabi")
# link_directories("/usr/local/lib")

# # Crow framework include path
# set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# # Include directories
# include_directories(
#     ${CROW_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/src
# )

# # Specify source files
# set(SOURCE_FILES
#     src/main.cpp
#     src/routes/listings.cpp
#     src/routes/user.cpp
#     src/controllers/listings/read_controller.cpp
#     src/controllers/listings/create_controller.cpp
#     src/controllers/listings/read_one_controller.cpp
#     src/controllers/listings/update_controller.cpp
#     src/controllers/listings/delete_controller.cpp
#     src/controllers/user/create_controller.cpp
#     src/controllers/user/read_controller.cpp
#     src/database/mongo_client.cpp
#     include/middleware.h
# )

# # Add executable
# add_executable(CrowServer ${SOURCE_FILES})

# # Find and link required packages
# find_package(Threads REQUIRED)  # Ensure Threads package is found
# find_package(Boost REQUIRED COMPONENTS system thread filesystem)  # Ensure Boost components are found

# # Link libraries
# target_link_libraries(CrowServer 
#     ${Boost_LIBRARIES} 
#     mongocxx 
#     bsoncxx 
#     Threads::Threads 
# )


