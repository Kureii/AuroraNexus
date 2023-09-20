cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_LIBRARY_NAME "${PROJECT_NAME}_lib")

include(./src/test-sources.cmake)

add_library(${TEST_LIBRARY_NAME} ${sources} ${headers})


set_target_properties(${TEST_LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(TEST_TARGET_NAME "${PROJECT_NAME}_tests")

add_executable(${TEST_TARGET_NAME} ${test_sources})

set_target_properties(${TEST_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

find_package(GTest REQUIRED)

target_link_libraries(${TEST_TARGET_NAME} ${TEST_LIBRARY_NAME} gtest_main gmock gtest)

target_compile_definitions(${TEST_TARGET_NAME} PRIVATE ENABLE_TESTS)
target_compile_definitions(${TEST_LIBRARY_NAME} PRIVATE ENABLE_TESTS)

enable_testing()
add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})
