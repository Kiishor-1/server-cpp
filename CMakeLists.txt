cmake_minimum_required(VERSION 3.10)
project(CrowServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify Boost include and library directories
set(BOOST_ROOT "C:/private/boost_1_85_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")

# Specify Crow library path
set(CROW_INCLUDE_DIR "C:/crow/include")


# Specify MongoDB C++ driver path
set(MONGO_CXX_DRIVER_ROOT "C:/mongo-cxx-driver")
set(MONGO_CXX_DRIVER_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include")
set(LIBBSONCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include/bsoncxx/v_noabi")
set(LIBMONGOCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include/mongocxx/v_noabi")
set(LIBMONGOCXX_LIB_DIR "${MONGO_CXX_DRIVER_ROOT}/lib")

# Include directories for Boost, Crow, routes, controllers, and middleware
include_directories(
    ${BOOST_INCLUDEDIR}
    ${CROW_INCLUDE_DIR}
    ${MONGO_CXX_DRIVER_INCLUDE_DIR}
    ${LIBMONGOCXX_INCLUDE_DIR}
    ${LIBBSONCXX_INCLUDE_DIR}
    ${LIBBSONCXX_INCLUDE_DIR}/bsoncxx/stdx
    ${LIBBSONCXX_INCLUDE_DIR}/bsoncxx/config
    ${LIBBSONCXX_INCLUDE_DIR}/bsoncxx/third_party/mnmlstc/core
    ${LIBMONGOCXX_INCLUDE_DIR}/mongocxx
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Link directories for Boost
# link_directories(${BOOST_LIBRARYDIR})
link_directories(${BOOST_LIBRARYDIR} ${LIBMONGOCXX_LIB_DIR})

# Specify Boost libraries
set(BOOST_LIBRARIES
    ${BOOST_LIBRARYDIR}/libboost_system-mgw14-mt-x64-1_85.a
    ${BOOST_LIBRARYDIR}/libboost_thread-mgw14-mt-x64-1_85.a
    ${BOOST_LIBRARYDIR}/libboost_filesystem-mgw14-mt-x64-1_85.a
    # Add more Boost libraries as needed
)

# Specify MongoDB libraries
set(MONGODB_LIBRARIES
    ${LIBMONGOCXX_LIB_DIR}/libmongocxx.dll.a
    ${LIBMONGOCXX_LIB_DIR}/libbsoncxx.dll.a
)

# Source files
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/routes/listings.cpp
    ${CMAKE_SOURCE_DIR}/src/routes/user.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/listings/read_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/listings/create_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/listings/read_one_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/listings/update_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/listings/delete_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/user/create_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/controllers/user/read_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/database/mongo_client.cpp
    # ${CMAKE_SOURCE_DIR}/include/stdx_optional.hpp
    # ${CMAKE_SOURCE_DIR}/include/middlewares/log_middleware.h
    ${CMAKE_SOURCE_DIR}/include/middleware.h
    # ${CMAKE_SOURCE_DIR}/include/middleware/is_logged_in.h
    # ${CMAKE_SOURCE_DIR}/include/middleware/is_author.h
    # ${CMAKE_SOURCE_DIR}/include/middleware/validate_listing.h
    # ${CMAKE_SOURCE_DIR}/include/middleware/validate_user.h
)

add_executable(CrowServer ${SOURCE_FILES})

# Link Boost libraries and Winsock library
# target_link_libraries(CrowServer ${BOOST_LIBRARIES} ws2_32 mswsock)
target_link_libraries(CrowServer ${BOOST_LIBRARIES} ${MONGODB_LIBRARIES} ws2_32 mswsock)
