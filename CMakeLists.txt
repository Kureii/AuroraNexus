cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Options
option(ENABLE_UNIT_TESTING "Enable unit tests?" ON)

# Project details
project(
  "AuroraNexus"
  VERSION 0.1.0
  LANGUAGES CXX
)

# Set project variables
include(include/include.cmake)
include(src/sources.cmake)

message(STATUS "Found the following sources:")

foreach(source IN LISTS exe_sources)
  message(STATUS "* ${source}")
endforeach()

foreach(source IN LISTS headers)
  message(STATUS "* ${source}")
endforeach()

include_directories(./include)

add_executable(${PROJECT_NAME}
  ${exe_sources}
  ${headers}
)

if(ENABLE_UNIT_TESTING)
  add_library(${PROJECT_NAME}_LIB ${headers} ${sources})
  set_target_properties(
    ${PROJECT_NAME}_LIB
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    OUTPUT_NAME ${PROJECT_NAME}
  )
endif()

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

# Packages
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Unit testing setup
if(ENABLE_UNIT_TESTING)
  enable_testing()
  message(STATUS "Build unit tests for the project. Tests should always be found in the test folder\n")
  add_subdirectory(test)
endif()
