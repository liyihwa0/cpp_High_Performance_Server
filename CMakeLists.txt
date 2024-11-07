cmake_minimum_required(VERSION 3.25)
project(learning)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-std=c++20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置构建类型为 Debug
set(CMAKE_BUILD_TYPE "Debug")

# 根据构建类型设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()



include_directories(.)
add_definitions(-rdynamic)

add_executable(learning
        buffer.h
        connection.h
        global_def.h
        epoll.h
        fsm.h
        http_parser.h
        locker.h
        main.cpp
        thread_pool.h
        thread.h
        wasignal.h
        logger/logger.h
        fiber.h)  # 注意 logger.h 的路径前面需要有空格
