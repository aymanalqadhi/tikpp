cmake_minimum_required(VERSION 3.10)

# Project name
project(tikpp CXX)

# Project options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set(LIBRARY_OUTPUT_PATH lib)

# Project files
set(SOURCE_FILES src/api.cpp)

# Targets
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

# Thiry-party libraries
add_subdirectory(lib/fmt)
add_subdirectory(lib/googletest)

# Testing
enable_testing()
add_subdirectory(tests)
