cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME AuroraNexus)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
