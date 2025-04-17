# 设置项目名称和语言标准
cmake_minimum_required(VERSION 3.25)
project(cpp_High_Performance_Server)

message(STATUS "c compiler ${CMAKE_C_COMPILER}")
message(STATUS "cpp compiler ${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器扩展


# ----------------------------------- 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


# ----------------------------------- 根据构建类型设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-elide-constructors")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
endif()

# ------------------------------------ 设置头文件根目录
include_directories(${CMAKE_SOURCE_DIR})
# ------------------------------------ 设置二进制文件目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# --------------------------------------- 查找 Protobuf 库
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)  # 查找 OpenSSL

# protobuf 生成文件 -------------------------------------
# 自动扫描所有 .proto 文件
file(GLOB_RECURSE PROTOBUF_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/*.proto")
message(STATUS "protobuf files: ${PROTOBUF_FILES}")

# 遍历每个proto文件，生成对应的.pb.cc和.pb.h文件到该proto文件所在的目录
foreach(PROTO_FILE ${PROTOBUF_FILES})
    # 获取proto文件的绝对路径
    set(ABS_PROTO_FILE "${CMAKE_SOURCE_DIR}/${PROTO_FILE}")
    # 获取proto文件的目录
    get_filename_component(PROTO_DIR ${ABS_PROTO_FILE} DIRECTORY)
    # 获取proto文件的名称（不含扩展名）
    get_filename_component(PROTO_FILE_NAME_WE ${PROTO_FILE} NAME_WE)

    # 设置生成的pb.cc和pb.h文件的输出路径
    set(GEN_PB_CC "${PROTO_DIR}/${PROTO_FILE_NAME_WE}.pb.cc")
    set(GEN_PB_H "${PROTO_DIR}/${PROTO_FILE_NAME_WE}.pb.h")
    message(STATUS "exec protoc --cpp_out=${PROTO_DIR}  -I=./${PROTO_DIR} ${ABS_PROTO_FILE}")

    # 使用execute_process在CMake配置阶段执行protoc命令
    execute_process(
            COMMAND protoc --cpp_out=${PROTO_DIR} -I=${PROTO_DIR} ${ABS_PROTO_FILE}
            OUTPUT_VARIABLE PROTOC_OUTPUT
            ERROR_VARIABLE PROTOC_ERROR
            RESULT_VARIABLE PROTOC_RESULT
    )

    # 检查protoc命令是否成功执行
    if(PROTOC_RESULT EQUAL 0)
        message(STATUS "Successfully generated ${ABS_PROTO_FILE}")
    else()
        message(FATAL_ERROR "Failed to generate ${ABS_PROTO_FILE} : ${PROTOC_ERROR}")
    endif()
endforeach()


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


# 拷贝静态目录到build目录下 --------------------------------------------------------------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/static DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "Copied static directory to ${CMAKE_BINARY_DIR}")