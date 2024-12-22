cmake_minimum_required(VERSION 3.10)
project(universal_neural_network_computing_accelerator_server)

set(CMAKE_CXX_STANDARD 11)

find_package(ZeroMQ REQUIRED)

include_directories(${ZeroMQ_INCLUDE_DIRS})

add_executable(server server.c detector.c)
