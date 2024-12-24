cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(neural-accelerator-server)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 手动查找 ZeroMQ
find_path(ZMQ_INCLUDE_DIR zmq.h REQUIRED PATHS /usr/include /usr/local/include)
find_library(ZMQ_LIBRARY zmq REQUIRED PATHS /usr/lib /usr/local/lib)

# 如果没找到 ZeroMQ，报错
if(NOT ZMQ_INCLUDE_DIR OR NOT ZMQ_LIBRARY)
    message(FATAL_ERROR "ZeroMQ not found. Please install it or specify the paths manually.")
endif()

# 查找 Protobuf
find_package(Protobuf REQUIRED)

# 添加可执行文件
add_executable(neural-accelerator-server
    accelerator-server.c
    api.pb-c.c
    detector.c
    clog.c
)

# 设置包含路径
target_include_directories(neural-accelerator-server PRIVATE
    ${ZMQ_INCLUDE_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

# 链接依赖库
target_link_libraries(neural-accelerator-server PRIVATE
    ${ZMQ_LIBRARY}
    ${Protobuf_LIBRARIES}
    m
    protobuf-c
)
# 添加可执行文件
add_executable(neural-accelerator-client
    accelerator-client.c
    api.pb-c.c
    detector.c
    clog.c
)
# 设置包含路径
target_include_directories(neural-accelerator-client PRIVATE
    ${ZMQ_INCLUDE_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

# 链接依赖库
target_link_libraries(neural-accelerator-client PRIVATE
    ${ZMQ_LIBRARY}
    ${Protobuf_LIBRARIES}
    m
    protobuf-c
)