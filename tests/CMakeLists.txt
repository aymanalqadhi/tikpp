cmake_minimum_required(VERSION 3.24)

include(CTest)

set(TESTS_ITERATIONS 1000000)
add_definitions(-DTEST_ITERATIONS=${TESTS_ITERATIONS})

include_directories(include/)

# Project functions
function(create_test test_name)
  add_executable(${test_name} src/${test_name}_tests.cpp)
  target_link_libraries(${test_name} PRIVATE tikpp gtest gtest_main)
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Project targets
create_test(testing_util_random)

create_test(convert)
create_test(convert_back)

create_test(basic_api)
create_test(request)
create_test(response)

create_test(operation_async_read_word_length)
create_test(operation_async_read_word)
create_test(operation_async_read_response)
create_test(operation_async_connect)

create_test(data_repository)
create_test(data_converter_proplist_collector)
create_test(data_converter_creator)
create_test(data_converter_dissolver)
create_test(data_query)
create_test(data_type_identity)
create_test(data_type_bytes)
create_test(data_type_read_only)
create_test(data_type_read_write)
create_test(data_type_duration)
create_test(data_type_stateless_value_wrapper)
create_test(data_type_stateful_value_wrapper)
create_test(data_type_stateful_wrapper)

create_test(command_add)
create_test(command_set)

# Test client
add_executable(test_client src/test_client.cpp)
target_link_libraries(test_client PRIVATE tikpp Boost::system)
