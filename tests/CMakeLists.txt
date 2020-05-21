cmake_minimum_required(VERSION 3.10)

# Project name
project(ezd_tests)

set(TESTS_ITERATIONS 1000000)
add_definitions(-DTEST_ITERATIONS=${TESTS_ITERATIONS})

# Project functions
function(create_test test_name)
    add_executable(${test_name} ${test_name}_tests.cpp)
    target_link_libraries(${test_name} ${MAIN_PROJECT_NAME} gtest gtest_main tikpp)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Project targets
create_test(basic_api)
create_test(request)
create_test(response)
