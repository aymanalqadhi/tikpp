cmake_minimum_required(VERSION 3.20)

# Project name
project(tikpp CXX)
set(MAIN_PROJECT_NAME ${PROJECT_NAME})

# Project options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# Thiry-party libraries
add_subdirectory(lib/fmt)
add_subdirectory(lib/googletest)
add_subdirectory(lib/openssl-cmake)

# Project files
set(SOURCE_FILES src/error.cpp
                 src/crypto.cpp
                 src/request.cpp
                 src/response.cpp)
include_directories(include/)

# Targets
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} fmt::fmt ssl crypto)
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
find_package(Boost COMPONENTS system thread REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})

# OpenSSL
set(SYSTEM_OPENSSL ON)
if (WIN32)
    link_directories(${OpenSSL_ROOT}/lib)
endif (WIN32)


# Testing (on *NIX operating systems only at the moment)
if (UNIX)
    enable_testing()
    add_subdirectory(tests)
endif (UNIX)