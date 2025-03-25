# 设置项目名称和语言标准
cmake_minimum_required(VERSION 3.25)
project(cpp_High_Performance_Server)

message(STATUS "c compiler ${CMAKE_C_COMPILER}")
message(STATUS "cpp compiler ${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器扩展


# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


# 根据构建类型设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-elide-constructors")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
endif()

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build-${CMAKE_BUILD_TYPE}")


include_directories(${CMAKE_SOURCE_DIR})

# 查找 Protobuf 库
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)  # 查找 OpenSSL


#设置搜索的头文件的路径,包含 Protobuf 的头文件
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(/usr/include)

# 设置项目库文件的搜索目录
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(/usr/local/lib)






# 搜集 src 目录下的所有 .cpp 文件
file(GLOB_RECURSE SRC_CPP_FILES "src/*.cpp" "src/*.cc")
# 排除 src/main 下的所有 .cpp 文件
list(FILTER SRC_CPP_FILES EXCLUDE REGEX ".*/src/main/.*\\.(cpp|cc)")


file(GLOB_RECURSE  SRC_H_FILES "src/*.h")
list(APPEND SRC_FILES ${SRC_H_FILES} ${SRC_CPP_FILES})
message(STATUS "src files: ${SRC_FILES}")

file(GLOB_RECURSE TEST_TARGET_FILES "test/*.cpp" "test/*.cc")
message(STATUS "test target files: ${TEST_TARGET_FILES}")

file(GLOB  TEST_H_FILES "test/*.h")
message(STATUS "test header files: ${TEST_H_FILES}")

file(GLOB_RECURSE MAIN_TARGET_FILES "src/main/*.cpp" "src/main/*.cc")
message(STATUS "main target files: ${MAIN_TARGET_FILES}")

file(GLOB_RECURSE EXAMPLE_TEMP_CPP_FILES "example/*.cpp" "example/*.cc")

set(EXAMPLE_TARGET_FILES "")
set(EXAMPLE_CPP_FILES "")
foreach(file ${EXAMPLE_TEMP_CPP_FILES})

    get_filename_component(filename ${file} NAME_WE)
    # 如果以example作为前缀
    string(FIND ${filename} "example" example_pos)

    if(example_pos STREQUAL 0)
        list(APPEND EXAMPLE_TARGET_FILES ${file})
    else()
        list(APPEND EXAMPLE_CPP_FILES ${file})
    endif()
endforeach()

message(STATUS "example target files: ${EXAMPLE_TARGET_FILES}")
message(STATUS "example cpp files: ${EXAMPLE_CPP_FILES}")

file(GLOB EXAMPLE_H_FILES "example/*.h")
message(STATUS "example header files: ${EXAMPLE_H_FILES}")


add_definitions(-rdynamic)

foreach(file ${TEST_TARGET_FILES})
    # 获取文件名（不含路径和扩展名）
    get_filename_component(target_name ${file} NAME_WE)

    # 创建可执行目标
    add_executable(${target_name} ${file} ${TEST_H_FILES} ${SRC_FILES} )

    # 打印目标名和对应的源文件
    message(STATUS "Created target: ${target_name} from ${file}")
endforeach()


foreach(file ${MAIN_TARGET_FILES})
    # 获取文件名（不含路径和扩展名）
    get_filename_component(target_name ${file} NAME_WE)

    # 创建可执行目标
    add_executable(${target_name} ${file} ${SRC_FILES})

    # 打印目标名和对应的源文件
    message(STATUS "Created target: ${target_name} from ${file}")
endforeach()

foreach(file ${EXAMPLE_TARGET_FILES})
    # 获取文件名（不含路径和扩展名）
    get_filename_component(target_name ${file} NAME_WE)

    # 创建可执行目标
    add_executable(${target_name} ${file} ${EXAMPLE_H_FILES} ${EXAMPLE_CPP_FILES} ${SRC_FILES})

    # 打印目标名和对应的源文件
    message(STATUS "Created target: ${target_name} from ${file}")

endforeach()


# 在创建目标之后，集中为所有目标链接 Protobuf 库
foreach(target IN LISTS TEST_TARGET_FILES MAIN_TARGET_FILES EXAMPLE_TARGET_FILES)
    get_filename_component(target_name ${target} NAME_WE)
    target_link_libraries(${target_name} ${Protobuf_LIBRARIES}  OpenSSL::SSL OpenSSL::Crypto pthread)
    message(STATUS "Linked Protobuf to target: ${target_name}")
endforeach()