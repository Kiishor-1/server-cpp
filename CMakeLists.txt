# cmake_minimum_required(VERSION 3.10)
# project(CrowServer)

# # C++ standard set karo
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Boost ka manually path set karo
# set(BOOST_ROOT "C:/local/boost_1_85_0")
# set(BOOST_LIBRARYDIR "C:/local/boost_1_85_0/stage/lib")
# include_directories("${BOOST_ROOT}")
# link_directories("${BOOST_LIBRARYDIR}")

# # MongoDB C++ driver paths set karo
# set(MONGO_CXX_DRIVER_ROOT "C:/mongo-cxx-driver")
# set(MONGO_CXX_DRIVER_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include")
# set(LIBBSONCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/bsoncxx/v_noabi")
# set(LIBMONGOCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/mongocxx/v_noabi")
# set(LIBMONGOCXX_LIB_DIR "${MONGO_CXX_DRIVER_ROOT}/lib")

# # Crow framework ka include path
# set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# # Include directories ke liye
# include_directories(
#     ${CROW_INCLUDE_DIR}
#     ${MONGO_CXX_DRIVER_INCLUDE_DIR}
#     ${LIBMONGOCXX_INCLUDE_DIR}
#     ${LIBBSONCXX_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/src
# )

# # Boost libraries ko specify karo (names matching tumhare mingw builds ke saath)
# set(Boost_LIBRARIES 
#     boost_system-mgw14-mt-x64-1_85 
#     boost_thread-mgw14-mt-x64-1_85 
#     boost_filesystem-mgw14-mt-x64-1_85)

# # MongoDB libraries ko specify karo
# set(MONGOCXX_LIBRARIES 
#     mongocxx
#     bsoncxx)

# # Threads package ko find karo
# find_package(Threads REQUIRED)

# # Source files ko specify karo
# set(SOURCE_FILES
#     ${CMAKE_SOURCE_DIR}/src/main.cpp
#     ${CMAKE_SOURCE_DIR}/src/routes/listings.cpp
#     ${CMAKE_SOURCE_DIR}/src/routes/user.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/read_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/create_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/read_one_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/update_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/listings/delete_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/user/create_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/controllers/user/read_controller.cpp
#     ${CMAKE_SOURCE_DIR}/src/database/mongo_client.cpp
#     ${CMAKE_SOURCE_DIR}/include/middleware.h
# )

# # Final executable build karo
# add_executable(CrowServer ${SOURCE_FILES})

# # Link directories for MongoDB C++ driver
# link_directories(${LIBMONGOCXX_LIB_DIR})

# # Libraries ko link karo
# target_link_libraries(CrowServer ${Boost_LIBRARIES} ${MONGOCXX_LIBRARIES} Threads::Threads)

cmake_minimum_required(VERSION 3.10)
project(CrowServer)

# C++ standard set
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost paths set
set(BOOST_ROOT "C:/local/boost_1_85_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_85_0/stage/lib")
include_directories("${BOOST_ROOT}")
link_directories("${BOOST_LIBRARYDIR}")

# MongoDB C++ driver paths set
set(MONGO_CXX_DRIVER_ROOT "C:/mongo-cxx-driver")
set(MONGO_CXX_DRIVER_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include")
set(LIBBSONCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/bsoncxx/v_noabi")
set(LIBMONGOCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/mongocxx/v_noabi")
set(LIBMONGOCXX_LIB_DIR "${MONGO_CXX_DRIVER_ROOT}/lib")

# Crow framework include path
set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# Include directories
include_directories(
    ${CROW_INCLUDE_DIR}
    ${MONGO_CXX_DRIVER_INCLUDE_DIR}
    ${LIBBSONCXX_INCLUDE_DIR}
    ${LIBMONGOCXX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Boost libraries
set(Boost_LIBRARIES 
    boost_system-mgw14-mt-x64-1_85 
    boost_thread-mgw14-mt-x64-1_85 
    boost_filesystem-mgw14-mt-x64-1_85)

# Add full library filenames for MongoDB libraries
set(MONGOCXX_LIBRARIES 
    "${LIBMONGOCXX_LIB_DIR}/libmongocxx.dll.a"
    "${LIBMONGOCXX_LIB_DIR}/libbsoncxx.dll.a")

# # MongoDB libraries
# set(MONGOCXX_LIBRARIES 
#     bsoncxx
#     mongocxx)

# Threads package find
find_package(Threads REQUIRED)

# Source files specify
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

# Final executable build
add_executable(CrowServer ${SOURCE_FILES})

# Libraries link
target_link_libraries(CrowServer ${Boost_LIBRARIES} ${MONGOCXX_LIBRARIES} Threads::Threads ws2_32 mswsock)
