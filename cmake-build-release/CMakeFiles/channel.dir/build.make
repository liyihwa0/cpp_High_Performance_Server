# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/build/cpp_High_Performance_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/build/cpp_High_Performance_Server/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/channel.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/channel.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/channel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/channel.dir/flags.make

CMakeFiles/channel.dir/test/std/channel.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/test/std/channel.cpp.o: /root/build/cpp_High_Performance_Server/test/std/channel.cpp
CMakeFiles/channel.dir/test/std/channel.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/channel.dir/test/std/channel.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/test/std/channel.cpp.o -MF CMakeFiles/channel.dir/test/std/channel.cpp.o.d -o CMakeFiles/channel.dir/test/std/channel.cpp.o -c /root/build/cpp_High_Performance_Server/test/std/channel.cpp

CMakeFiles/channel.dir/test/std/channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/test/std/channel.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/test/std/channel.cpp > CMakeFiles/channel.dir/test/std/channel.cpp.i

CMakeFiles/channel.dir/test/std/channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/test/std/channel.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/test/std/channel.cpp -o CMakeFiles/channel.dir/test/std/channel.cpp.s

CMakeFiles/channel.dir/src/fiber/fiber.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/fiber/fiber.cpp.o: /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp
CMakeFiles/channel.dir/src/fiber/fiber.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/channel.dir/src/fiber/fiber.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/fiber/fiber.cpp.o -MF CMakeFiles/channel.dir/src/fiber/fiber.cpp.o.d -o CMakeFiles/channel.dir/src/fiber/fiber.cpp.o -c /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp

CMakeFiles/channel.dir/src/fiber/fiber.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/fiber/fiber.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp > CMakeFiles/channel.dir/src/fiber/fiber.cpp.i

CMakeFiles/channel.dir/src/fiber/fiber.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/fiber/fiber.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp -o CMakeFiles/channel.dir/src/fiber/fiber.cpp.s

CMakeFiles/channel.dir/src/http/http_common.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/http/http_common.cpp.o: /root/build/cpp_High_Performance_Server/src/http/http_common.cpp
CMakeFiles/channel.dir/src/http/http_common.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/channel.dir/src/http/http_common.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/http/http_common.cpp.o -MF CMakeFiles/channel.dir/src/http/http_common.cpp.o.d -o CMakeFiles/channel.dir/src/http/http_common.cpp.o -c /root/build/cpp_High_Performance_Server/src/http/http_common.cpp

CMakeFiles/channel.dir/src/http/http_common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/http/http_common.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/http/http_common.cpp > CMakeFiles/channel.dir/src/http/http_common.cpp.i

CMakeFiles/channel.dir/src/http/http_common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/http/http_common.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/http/http_common.cpp -o CMakeFiles/channel.dir/src/http/http_common.cpp.s

CMakeFiles/channel.dir/src/http/request_parser.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/http/request_parser.cpp.o: /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp
CMakeFiles/channel.dir/src/http/request_parser.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/channel.dir/src/http/request_parser.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/http/request_parser.cpp.o -MF CMakeFiles/channel.dir/src/http/request_parser.cpp.o.d -o CMakeFiles/channel.dir/src/http/request_parser.cpp.o -c /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp

CMakeFiles/channel.dir/src/http/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/http/request_parser.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp > CMakeFiles/channel.dir/src/http/request_parser.cpp.i

CMakeFiles/channel.dir/src/http/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/http/request_parser.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp -o CMakeFiles/channel.dir/src/http/request_parser.cpp.s

CMakeFiles/channel.dir/src/locker/locker.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/locker/locker.cpp.o: /root/build/cpp_High_Performance_Server/src/locker/locker.cpp
CMakeFiles/channel.dir/src/locker/locker.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/channel.dir/src/locker/locker.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/locker/locker.cpp.o -MF CMakeFiles/channel.dir/src/locker/locker.cpp.o.d -o CMakeFiles/channel.dir/src/locker/locker.cpp.o -c /root/build/cpp_High_Performance_Server/src/locker/locker.cpp

CMakeFiles/channel.dir/src/locker/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/locker/locker.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/locker/locker.cpp > CMakeFiles/channel.dir/src/locker/locker.cpp.i

CMakeFiles/channel.dir/src/locker/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/locker/locker.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/locker/locker.cpp -o CMakeFiles/channel.dir/src/locker/locker.cpp.s

CMakeFiles/channel.dir/src/logger/logger.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/logger/logger.cpp.o: /root/build/cpp_High_Performance_Server/src/logger/logger.cpp
CMakeFiles/channel.dir/src/logger/logger.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/channel.dir/src/logger/logger.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/logger/logger.cpp.o -MF CMakeFiles/channel.dir/src/logger/logger.cpp.o.d -o CMakeFiles/channel.dir/src/logger/logger.cpp.o -c /root/build/cpp_High_Performance_Server/src/logger/logger.cpp

CMakeFiles/channel.dir/src/logger/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/logger/logger.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/logger/logger.cpp > CMakeFiles/channel.dir/src/logger/logger.cpp.i

CMakeFiles/channel.dir/src/logger/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/logger/logger.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/logger/logger.cpp -o CMakeFiles/channel.dir/src/logger/logger.cpp.s

CMakeFiles/channel.dir/src/net/epoll.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/net/epoll.cpp.o: /root/build/cpp_High_Performance_Server/src/net/epoll.cpp
CMakeFiles/channel.dir/src/net/epoll.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/channel.dir/src/net/epoll.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/net/epoll.cpp.o -MF CMakeFiles/channel.dir/src/net/epoll.cpp.o.d -o CMakeFiles/channel.dir/src/net/epoll.cpp.o -c /root/build/cpp_High_Performance_Server/src/net/epoll.cpp

CMakeFiles/channel.dir/src/net/epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/net/epoll.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/net/epoll.cpp > CMakeFiles/channel.dir/src/net/epoll.cpp.i

CMakeFiles/channel.dir/src/net/epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/net/epoll.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/net/epoll.cpp -o CMakeFiles/channel.dir/src/net/epoll.cpp.s

CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o: /root/build/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc
CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o -MF CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o.d -o CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o -c /root/build/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc

CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc > CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.i

CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc -o CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.s

CMakeFiles/channel.dir/src/raft/raft_core.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/raft/raft_core.cpp.o: /root/build/cpp_High_Performance_Server/src/raft/raft_core.cpp
CMakeFiles/channel.dir/src/raft/raft_core.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/channel.dir/src/raft/raft_core.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/raft/raft_core.cpp.o -MF CMakeFiles/channel.dir/src/raft/raft_core.cpp.o.d -o CMakeFiles/channel.dir/src/raft/raft_core.cpp.o -c /root/build/cpp_High_Performance_Server/src/raft/raft_core.cpp

CMakeFiles/channel.dir/src/raft/raft_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/raft/raft_core.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/raft/raft_core.cpp > CMakeFiles/channel.dir/src/raft/raft_core.cpp.i

CMakeFiles/channel.dir/src/raft/raft_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/raft/raft_core.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/raft/raft_core.cpp -o CMakeFiles/channel.dir/src/raft/raft_core.cpp.s

CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o: /root/build/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc
CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o -MF CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o.d -o CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o -c /root/build/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc

CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc > CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.i

CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc -o CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.s

CMakeFiles/channel.dir/src/thread/thread.cpp.o: CMakeFiles/channel.dir/flags.make
CMakeFiles/channel.dir/src/thread/thread.cpp.o: /root/build/cpp_High_Performance_Server/src/thread/thread.cpp
CMakeFiles/channel.dir/src/thread/thread.cpp.o: CMakeFiles/channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/channel.dir/src/thread/thread.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/channel.dir/src/thread/thread.cpp.o -MF CMakeFiles/channel.dir/src/thread/thread.cpp.o.d -o CMakeFiles/channel.dir/src/thread/thread.cpp.o -c /root/build/cpp_High_Performance_Server/src/thread/thread.cpp

CMakeFiles/channel.dir/src/thread/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/channel.dir/src/thread/thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/thread/thread.cpp > CMakeFiles/channel.dir/src/thread/thread.cpp.i

CMakeFiles/channel.dir/src/thread/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/channel.dir/src/thread/thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/thread/thread.cpp -o CMakeFiles/channel.dir/src/thread/thread.cpp.s

# Object files for target channel
channel_OBJECTS = \
"CMakeFiles/channel.dir/test/std/channel.cpp.o" \
"CMakeFiles/channel.dir/src/fiber/fiber.cpp.o" \
"CMakeFiles/channel.dir/src/http/http_common.cpp.o" \
"CMakeFiles/channel.dir/src/http/request_parser.cpp.o" \
"CMakeFiles/channel.dir/src/locker/locker.cpp.o" \
"CMakeFiles/channel.dir/src/logger/logger.cpp.o" \
"CMakeFiles/channel.dir/src/net/epoll.cpp.o" \
"CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o" \
"CMakeFiles/channel.dir/src/raft/raft_core.cpp.o" \
"CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o" \
"CMakeFiles/channel.dir/src/thread/thread.cpp.o"

# External object files for target channel
channel_EXTERNAL_OBJECTS =

/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/test/std/channel.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/fiber/fiber.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/http/http_common.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/http/request_parser.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/locker/locker.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/logger/logger.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/net/epoll.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/net/rpc/rpc_framework.pb.cc.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/raft/raft_core.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/raft/raft_core_proto.pb.cc.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/src/thread/thread.cpp.o
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/build.make
/root/build/cpp_High_Performance_Server/bin/Release/channel: /usr/local/lib/libprotobuf.so
/root/build/cpp_High_Performance_Server/bin/Release/channel: /usr/lib/x86_64-linux-gnu/libssl.so
/root/build/cpp_High_Performance_Server/bin/Release/channel: /usr/lib/x86_64-linux-gnu/libcrypto.so
/root/build/cpp_High_Performance_Server/bin/Release/channel: CMakeFiles/channel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable /root/build/cpp_High_Performance_Server/bin/Release/channel"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/channel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/channel.dir/build: /root/build/cpp_High_Performance_Server/bin/Release/channel
.PHONY : CMakeFiles/channel.dir/build

CMakeFiles/channel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/channel.dir/cmake_clean.cmake
.PHONY : CMakeFiles/channel.dir/clean

CMakeFiles/channel.dir/depend:
	cd /root/build/cpp_High_Performance_Server/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/build/cpp_High_Performance_Server /root/build/cpp_High_Performance_Server /root/build/cpp_High_Performance_Server/cmake-build-release /root/build/cpp_High_Performance_Server/cmake-build-release /root/build/cpp_High_Performance_Server/cmake-build-release/CMakeFiles/channel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/channel.dir/depend

