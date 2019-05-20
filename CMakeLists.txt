
cmake_minimum_required(VERSION 3.13)
project(client_network_module)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align -Wcast-qual -Wunreachable-code -L/usr/local/ssl/lib -lssl -lcrypto")

set(SOURCE_FILES
        src/ClientNetwork_SSL.hh
        src/ClientNetwork_SSL.cpp
        sample/Connection.cpp
        sample/Connection.hh
        sample/ssl_sample/HttpsConnection.cpp
        sample/ssl_sample/HttpsConnection.hh
        sample/main.cpp)

add_executable(client_network_module ${SOURCE_FILES})