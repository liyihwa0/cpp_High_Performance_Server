# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /usr/local/cmake-3.26.0/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake-3.26.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.hmUOgZAfsx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.hmUOgZAfsx/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/thread_channel.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/thread_channel.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/thread_channel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/thread_channel.dir/flags.make

CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o: /tmp/tmp.hmUOgZAfsx/test/std/thread_channel.cpp
CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o -MF CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o.d -o CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o -c /tmp/tmp.hmUOgZAfsx/test/std/thread_channel.cpp

CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/test/std/thread_channel.cpp > CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.i

CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/test/std/thread_channel.cpp -o CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.s

CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o: /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp
CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o -MF CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o.d -o CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp

CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp > CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.i

CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp -o CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.s

CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o: /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp
CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o -MF CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o.d -o CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp

CMakeFiles/thread_channel.dir/src/http/http_common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/http/http_common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp > CMakeFiles/thread_channel.dir/src/http/http_common.cpp.i

CMakeFiles/thread_channel.dir/src/http/http_common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/http/http_common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp -o CMakeFiles/thread_channel.dir/src/http/http_common.cpp.s

CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o: /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp
CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o -MF CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o.d -o CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp

CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp > CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.i

CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp -o CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.s

CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o: /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp
CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o -MF CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o.d -o CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp

CMakeFiles/thread_channel.dir/src/locker/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/locker/locker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp > CMakeFiles/thread_channel.dir/src/locker/locker.cpp.i

CMakeFiles/thread_channel.dir/src/locker/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/locker/locker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp -o CMakeFiles/thread_channel.dir/src/locker/locker.cpp.s

CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o: /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp
CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o -MF CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o.d -o CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp

CMakeFiles/thread_channel.dir/src/logger/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/logger/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp > CMakeFiles/thread_channel.dir/src/logger/logger.cpp.i

CMakeFiles/thread_channel.dir/src/logger/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/logger/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp -o CMakeFiles/thread_channel.dir/src/logger/logger.cpp.s

CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o: /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp
CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o -MF CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o.d -o CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp

CMakeFiles/thread_channel.dir/src/net/epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/net/epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp > CMakeFiles/thread_channel.dir/src/net/epoll.cpp.i

CMakeFiles/thread_channel.dir/src/net/epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/net/epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp -o CMakeFiles/thread_channel.dir/src/net/epoll.cpp.s

CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o: /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc
CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o -MF CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o.d -o CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o -c /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc

CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc > CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.i

CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc -o CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.s

CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o: /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp
CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o -MF CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o.d -o CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp

CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp > CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.i

CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp -o CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.s

CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o: /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc
CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o -MF CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o.d -o CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o -c /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc

CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc > CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.i

CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc -o CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.s

CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o: CMakeFiles/thread_channel.dir/flags.make
CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o: /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp
CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o: CMakeFiles/thread_channel.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o -MF CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o.d -o CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp

CMakeFiles/thread_channel.dir/src/thread/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_channel.dir/src/thread/thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp > CMakeFiles/thread_channel.dir/src/thread/thread.cpp.i

CMakeFiles/thread_channel.dir/src/thread/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_channel.dir/src/thread/thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp -o CMakeFiles/thread_channel.dir/src/thread/thread.cpp.s

# Object files for target thread_channel
thread_channel_OBJECTS = \
"CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o" \
"CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o" \
"CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o" \
"CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o" \
"CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o" \
"CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o" \
"CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o" \
"CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o" \
"CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o" \
"CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o" \
"CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o"

# External object files for target thread_channel
thread_channel_EXTERNAL_OBJECTS =

thread_channel: CMakeFiles/thread_channel.dir/test/std/thread_channel.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/fiber/fiber.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/http/http_common.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/http/request_parser.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/locker/locker.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/logger/logger.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/net/epoll.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/net/rpc/rpc_framework.pb.cc.o
thread_channel: CMakeFiles/thread_channel.dir/src/raft/raft_core.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/src/raft/raft_core_proto.pb.cc.o
thread_channel: CMakeFiles/thread_channel.dir/src/thread/thread.cpp.o
thread_channel: CMakeFiles/thread_channel.dir/build.make
thread_channel: /usr/local/lib/libprotobuf.so
thread_channel: /usr/lib64/libssl.so
thread_channel: /usr/lib64/libcrypto.so
thread_channel: CMakeFiles/thread_channel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable thread_channel"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thread_channel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/thread_channel.dir/build: thread_channel
.PHONY : CMakeFiles/thread_channel.dir/build

CMakeFiles/thread_channel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/thread_channel.dir/cmake_clean.cmake
.PHONY : CMakeFiles/thread_channel.dir/clean

CMakeFiles/thread_channel.dir/depend:
	cd /tmp/tmp.hmUOgZAfsx/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.hmUOgZAfsx /tmp/tmp.hmUOgZAfsx /tmp/tmp.hmUOgZAfsx/cmake-build-debug /tmp/tmp.hmUOgZAfsx/cmake-build-debug /tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles/thread_channel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/thread_channel.dir/depend

