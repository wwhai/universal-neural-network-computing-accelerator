cmake_minimum_required(VERSION 3.10)

project(neural-accelerator-server)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output")
#===================================================================================================
# 添加Server源码
#===================================================================================================
aux_source_directory(${SOURCE_DIR} SRC_FILES_SERVER)
message(STATUS "SRC_FILES_SERVER: ${SRC_FILES_SERVER}")
#===================================================================================================
# 添加Client源码
#===================================================================================================
aux_source_directory(${SOURCE_DIR} SRC_FILES_CLIENT)
message(STATUS "SRC_FILES_CLIENT: ${SRC_FILES_CLIENT}")
# 手动查找 ZeroMQ
find_path(ZMQ_INCLUDE_DIR zmq.h REQUIRED PATHS /usr/include /usr/local/include)
find_library(ZMQ_LIBRARY zmq REQUIRED PATHS /usr/lib /usr/local/lib)

if(NOT ZMQ_INCLUDE_DIR OR NOT ZMQ_LIBRARY)
    message(FATAL_ERROR "ZeroMQ not found. Please install it or specify the paths manually.")
endif()

# 查找 Protobuf
find_package(Protobuf REQUIRED)

#===================================================================================================
# 添加可执行文件
#===================================================================================================
set(FILE_TO_EXCLUDE_SERVER "${SOURCE_DIR}/accelerator-client.c")
list(REMOVE_ITEM SRC_FILES_SERVER ${FILE_TO_EXCLUDE_SERVER})
add_executable(neural-accelerator-server ${SRC_FILES_SERVER})

# 设置包含路径
target_include_directories(neural-accelerator-server PRIVATE
    ${ZMQ_INCLUDE_DIR}
    ${Protobuf_INCLUDE_DIRS}
    ${SOURCE_DIR}
)

# 链接依赖库
target_link_libraries(neural-accelerator-server PRIVATE
    ${ZMQ_LIBRARY}
    ${Protobuf_LIBRARIES}
    m
    protobuf-c
)
#===================================================================================================
# 添加可执行文件
#===================================================================================================
set(FILE_TO_EXCLUDE_CLIENT "${SOURCE_DIR}/accelerator-server.c")
list(REMOVE_ITEM SRC_FILES_CLIENT ${FILE_TO_EXCLUDE_CLIENT})
add_executable(neural-accelerator-client ${SRC_FILES_CLIENT})
# 设置包含路径
target_include_directories(neural-accelerator-client PRIVATE
    ${ZMQ_INCLUDE_DIR}
    ${Protobuf_INCLUDE_DIRS}
    ${SOURCE_DIR}
)

# 链接依赖库
target_link_libraries(neural-accelerator-client PRIVATE
    ${ZMQ_LIBRARY}
    ${Protobuf_LIBRARIES}
    m
    protobuf-c
)
