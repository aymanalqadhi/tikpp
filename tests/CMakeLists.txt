cmake_minimum_required(VERSION 3.10)

# Project name
project(ezd_tests)

set(TESTS_ITERATIONS 1000000)
add_definitions(-DTEST_ITERATIONS=${TESTS_ITERATIONS})

include_directories(include/)

# Project functions
function(create_test test_name)
    add_executable(${test_name} src/${test_name}_tests.cpp)
    target_link_libraries(${test_name} ${MAIN_PROJECT_NAME}
                                       gtest
                                       gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Project targets
create_test(util_random)

create_test(convert)
create_test(convert_back)

create_test(request)
create_test(response)
create_test(async_read_word_length)
create_test(async_read_word)
create_test(async_read_response)
create_test(async_connect)

create_test(data_converter_proplist_collector)
create_test(data_converter_creator)
create_test(data_converter_dissolver)
create_test(data_query)
create_test(data_type_identity)
create_test(data_type_bytes)
create_test(data_type_one_way)
create_test(data_type_two_way)
create_test(data_type_duration)

create_test(commands_add)
create_test(commands_set)

# Test client
add_executable(test_client src/test_client.cpp)
find_package(Boost COMPONENTS system thread REQUIRED)
target_link_libraries(test_client tikpp ${Boost_LIBRARIES})
