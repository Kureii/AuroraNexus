cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME AuroraNexus)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED)

# MySQL
find_library(MYSQLCPPCONN_LIBRARY
        NAMES mysqlcppconn
        HINTS "/usr/lib/x86_64-linux-gnu")
find_path(MYSQLCPPCONN_INCLUDE_DIR
        NAMES mysql_driver.h
        HINTS "/usr/include/cppconn")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/interfaces/database_connection.hpp
        src/infrastructure/database_repositories/aurora_mysql_connection.hpp
        src/infrastructure/database_repositories/database_service.hpp
        )

include_directories(${Boost_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${MYSQLCPPCONN_INCLUDE_DIR}
        )

target_link_libraries(${PROJECT_NAME} ${MYSQLCPPCONN_LIBRARY})