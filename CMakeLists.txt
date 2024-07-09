cmake_minimum_required(VERSION 3.10)
project(CrowServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost 1.65 REQUIRED COMPONENTS system thread filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# Specify Crow include directory
set(CROW_INCLUDE_DIR "/usr/local/include/crow")

# Specify MongoDB C++ driver paths
set(MONGO_CXX_DRIVER_ROOT "/usr/local")
set(MONGO_CXX_DRIVER_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include")
set(LIBBSONCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/bsoncxx/v_noabi")
set(LIBMONGOCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/mongocxx/v_noabi")
set(LIBMONGOCXX_LIB_DIR "${MONGO_CXX_DRIVER_ROOT}/lib")

# Include directories for Boost, Crow, and MongoDB C++ driver
include_directories(
    ${CROW_INCLUDE_DIR}
    ${MONGO_CXX_DRIVER_INCLUDE_DIR}
    ${LIBMONGOCXX_INCLUDE_DIR}
    ${LIBBSONCXX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Link directories for Boost and MongoDB C++ driver
link_directories(${LIBMONGOCXX_LIB_DIR})

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
    ${CMAKE_SOURCE_DIR}/include/middleware.h
)

add_executable(CrowServer ${SOURCE_FILES})

# Link Boost and MongoDB libraries
target_link_libraries(CrowServer ${Boost_LIBRARIES} mongocxx bsoncxx)
