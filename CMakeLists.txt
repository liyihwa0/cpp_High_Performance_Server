cmake_minimum_required(VERSION 3.25)
project(cpp_High_Performance_Server)

set(CMAKE_CXX_STANDARD 20)
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



# 头文件搜索路径
include_directories(.)
include_directories(test)

file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB HEADER_FILES "src/*.h")
file(GLOB TEST_SOURCE_FILES "test/*.cpp")
file(GLOB TEST_HEADER_FILES "test/*.h")



# 源文件搜索路径
#add_library(MyLib   src/mylib.cpp)

#创建一个可执行文件目标 MyExecutable
# add_executable(MyExecutable src/main.cpp)

# 设置编译后的可执行文件路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-rdynamic)

add_executable(
        fiber_test
        test/fiber_test.cpp
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${TEST_HEADER_FILES})

add_executable(
        uni_ptr_test
        test/uni_ptr_test.cpp
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${TEST_HEADER_FILES})
