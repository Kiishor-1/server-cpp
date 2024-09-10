# cmake_minimum_required(VERSION 3.10)
# project(CrowServer)

# # C++ standard set
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Boost paths set
# set(BOOST_ROOT "C:/local/boost_1_85_0")
# set(BOOST_LIBRARYDIR "C:/local/boost_1_85_0/stage/lib")
# include_directories("${BOOST_ROOT}")
# link_directories("${BOOST_LIBRARYDIR}")

# # MongoDB C++ driver paths set
# set(MONGO_CXX_DRIVER_ROOT "C:/mongo-cxx-driver")
# set(MONGO_CXX_DRIVER_INCLUDE_DIR "${MONGO_CXX_DRIVER_ROOT}/include")
# set(LIBBSONCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/bsoncxx/v_noabi")
# set(LIBMONGOCXX_INCLUDE_DIR "${MONGO_CXX_DRIVER_INCLUDE_DIR}/mongocxx/v_noabi")
# set(LIBMONGOCXX_LIB_DIR "${MONGO_CXX_DRIVER_ROOT}/lib")

# # Crow framework include path
# set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# # Include directories
# include_directories(
#     ${CROW_INCLUDE_DIR}
#     ${MONGO_CXX_DRIVER_INCLUDE_DIR}
#     ${LIBBSONCXX_INCLUDE_DIR}
#     ${LIBMONGOCXX_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/src
# )

# # Boost libraries
# set(Boost_LIBRARIES 
#     boost_system-mgw14-mt-x64-1_85 
#     boost_thread-mgw14-mt-x64-1_85 
#     boost_filesystem-mgw14-mt-x64-1_85)

# # Add full library filenames for MongoDB libraries
# set(MONGOCXX_LIBRARIES 
#     "${LIBMONGOCXX_LIB_DIR}/libmongocxx.dll.a"
#     "${LIBMONGOCXX_LIB_DIR}/libbsoncxx.dll.a")

# # # MongoDB libraries
# # set(MONGOCXX_LIBRARIES 
# #     bsoncxx
# #     mongocxx)

# # Threads package find
# find_package(Threads REQUIRED)

# # Source files specify
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

# # Final executable build
# add_executable(CrowServer ${SOURCE_FILES})

# # Libraries link
# target_link_libraries(CrowServer ${Boost_LIBRARIES} ${MONGOCXX_LIBRARIES} Threads::Threads ws2_32 mswsock)



cmake_minimum_required(VERSION 3.10)
project(CrowServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly set Boost paths (adjust the paths if necessary)
set(BOOST_ROOT "/usr/include")
set(BOOST_LIBRARYDIR "/usr/lib")

# Manually include Boost directories
include_directories(${BOOST_ROOT})
link_directories(${BOOST_LIBRARYDIR})

# Specify Boost libraries
set(Boost_LIBRARIES
    boost_system
    boost_thread
    boost_filesystem
)

# MongoDB C++ driver paths (assuming it's installed in Docker)
include_directories("/usr/local/include/mongocxx/v_noabi" "/usr/local/include/bsoncxx/v_noabi")
link_directories("/usr/local/lib")

# Crow framework include path
set(CROW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/crow")

# Include directories
include_directories(
    ${CROW_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Specify source files
set(SOURCE_FILES
    src/main.cpp
    src/routes/listings.cpp
    src/routes/user.cpp
    src/controllers/listings/read_controller.cpp
    src/controllers/listings/create_controller.cpp
    src/controllers/listings/read_one_controller.cpp
    src/controllers/listings/update_controller.cpp
    src/controllers/listings/delete_controller.cpp
    src/controllers/user/create_controller.cpp
    src/controllers/user/read_controller.cpp
    src/database/mongo_client.cpp
    include/middleware.h
)

# Add executable
add_executable(CrowServer ${SOURCE_FILES})

# Find and link required packages
find_package(Threads REQUIRED)  # Ensure Threads package is found
find_package(Boost REQUIRED COMPONENTS system thread filesystem)  # Ensure Boost components are found

# Link libraries
target_link_libraries(CrowServer 
    ${Boost_LIBRARIES} 
    mongocxx 
    bsoncxx 
    Threads::Threads 
)


