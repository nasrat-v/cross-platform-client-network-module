
cmake_minimum_required(VERSION 3.13)
project(client_network_module)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align -Wcast-qual -Wunreachable-code")

set(SOURCE_FILES
        src/ClientNetwork.hh
        src/ClientNetwork.cpp
        src/Log.hh
        src/Log.cpp
        sample/Communication.cpp
        sample/Communication.hh
        sample/main.cpp)

add_executable(client_network_module ${SOURCE_FILES})