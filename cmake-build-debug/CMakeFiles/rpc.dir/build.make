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
include CMakeFiles/rpc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rpc.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rpc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rpc.dir/flags.make

CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o: /tmp/tmp.hmUOgZAfsx/src/main/raft_kvserver/rpc.pb.cc
CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o -MF CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o.d -o CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o -c /tmp/tmp.hmUOgZAfsx/src/main/raft_kvserver/rpc.pb.cc

CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/main/raft_kvserver/rpc.pb.cc > CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.i

CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/main/raft_kvserver/rpc.pb.cc -o CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.s

CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o: /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp
CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o -MF CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o.d -o CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp

CMakeFiles/rpc.dir/src/fiber/fiber.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/fiber/fiber.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp > CMakeFiles/rpc.dir/src/fiber/fiber.cpp.i

CMakeFiles/rpc.dir/src/fiber/fiber.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/fiber/fiber.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/fiber/fiber.cpp -o CMakeFiles/rpc.dir/src/fiber/fiber.cpp.s

CMakeFiles/rpc.dir/src/http/http_common.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/http/http_common.cpp.o: /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp
CMakeFiles/rpc.dir/src/http/http_common.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/rpc.dir/src/http/http_common.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/http/http_common.cpp.o -MF CMakeFiles/rpc.dir/src/http/http_common.cpp.o.d -o CMakeFiles/rpc.dir/src/http/http_common.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp

CMakeFiles/rpc.dir/src/http/http_common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/http/http_common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp > CMakeFiles/rpc.dir/src/http/http_common.cpp.i

CMakeFiles/rpc.dir/src/http/http_common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/http/http_common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/http/http_common.cpp -o CMakeFiles/rpc.dir/src/http/http_common.cpp.s

CMakeFiles/rpc.dir/src/http/request_parser.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/http/request_parser.cpp.o: /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp
CMakeFiles/rpc.dir/src/http/request_parser.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/rpc.dir/src/http/request_parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/http/request_parser.cpp.o -MF CMakeFiles/rpc.dir/src/http/request_parser.cpp.o.d -o CMakeFiles/rpc.dir/src/http/request_parser.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp

CMakeFiles/rpc.dir/src/http/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/http/request_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp > CMakeFiles/rpc.dir/src/http/request_parser.cpp.i

CMakeFiles/rpc.dir/src/http/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/http/request_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/http/request_parser.cpp -o CMakeFiles/rpc.dir/src/http/request_parser.cpp.s

CMakeFiles/rpc.dir/src/locker/locker.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/locker/locker.cpp.o: /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp
CMakeFiles/rpc.dir/src/locker/locker.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/rpc.dir/src/locker/locker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/locker/locker.cpp.o -MF CMakeFiles/rpc.dir/src/locker/locker.cpp.o.d -o CMakeFiles/rpc.dir/src/locker/locker.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp

CMakeFiles/rpc.dir/src/locker/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/locker/locker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp > CMakeFiles/rpc.dir/src/locker/locker.cpp.i

CMakeFiles/rpc.dir/src/locker/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/locker/locker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/locker/locker.cpp -o CMakeFiles/rpc.dir/src/locker/locker.cpp.s

CMakeFiles/rpc.dir/src/logger/logger.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/logger/logger.cpp.o: /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp
CMakeFiles/rpc.dir/src/logger/logger.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/rpc.dir/src/logger/logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/logger/logger.cpp.o -MF CMakeFiles/rpc.dir/src/logger/logger.cpp.o.d -o CMakeFiles/rpc.dir/src/logger/logger.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp

CMakeFiles/rpc.dir/src/logger/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/logger/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp > CMakeFiles/rpc.dir/src/logger/logger.cpp.i

CMakeFiles/rpc.dir/src/logger/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/logger/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/logger/logger.cpp -o CMakeFiles/rpc.dir/src/logger/logger.cpp.s

CMakeFiles/rpc.dir/src/net/epoll.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/net/epoll.cpp.o: /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp
CMakeFiles/rpc.dir/src/net/epoll.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/rpc.dir/src/net/epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/net/epoll.cpp.o -MF CMakeFiles/rpc.dir/src/net/epoll.cpp.o.d -o CMakeFiles/rpc.dir/src/net/epoll.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp

CMakeFiles/rpc.dir/src/net/epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/net/epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp > CMakeFiles/rpc.dir/src/net/epoll.cpp.i

CMakeFiles/rpc.dir/src/net/epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/net/epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/net/epoll.cpp -o CMakeFiles/rpc.dir/src/net/epoll.cpp.s

CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o: /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc
CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o -MF CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o.d -o CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o -c /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc

CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc > CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.i

CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/net/rpc/rpc_framework.pb.cc -o CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.s

CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o: /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp
CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o -MF CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o.d -o CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp

CMakeFiles/rpc.dir/src/raft/raft_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/raft/raft_core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp > CMakeFiles/rpc.dir/src/raft/raft_core.cpp.i

CMakeFiles/rpc.dir/src/raft/raft_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/raft/raft_core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/raft/raft_core.cpp -o CMakeFiles/rpc.dir/src/raft/raft_core.cpp.s

CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o: /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc
CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o -MF CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o.d -o CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o -c /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc

CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc > CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.i

CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/raft/raft_core_proto.pb.cc -o CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.s

CMakeFiles/rpc.dir/src/thread/thread.cpp.o: CMakeFiles/rpc.dir/flags.make
CMakeFiles/rpc.dir/src/thread/thread.cpp.o: /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp
CMakeFiles/rpc.dir/src/thread/thread.cpp.o: CMakeFiles/rpc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/rpc.dir/src/thread/thread.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rpc.dir/src/thread/thread.cpp.o -MF CMakeFiles/rpc.dir/src/thread/thread.cpp.o.d -o CMakeFiles/rpc.dir/src/thread/thread.cpp.o -c /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp

CMakeFiles/rpc.dir/src/thread/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc.dir/src/thread/thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp > CMakeFiles/rpc.dir/src/thread/thread.cpp.i

CMakeFiles/rpc.dir/src/thread/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc.dir/src/thread/thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.hmUOgZAfsx/src/thread/thread.cpp -o CMakeFiles/rpc.dir/src/thread/thread.cpp.s

# Object files for target rpc
rpc_OBJECTS = \
"CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o" \
"CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o" \
"CMakeFiles/rpc.dir/src/http/http_common.cpp.o" \
"CMakeFiles/rpc.dir/src/http/request_parser.cpp.o" \
"CMakeFiles/rpc.dir/src/locker/locker.cpp.o" \
"CMakeFiles/rpc.dir/src/logger/logger.cpp.o" \
"CMakeFiles/rpc.dir/src/net/epoll.cpp.o" \
"CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o" \
"CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o" \
"CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o" \
"CMakeFiles/rpc.dir/src/thread/thread.cpp.o"

# External object files for target rpc
rpc_EXTERNAL_OBJECTS =

rpc: CMakeFiles/rpc.dir/src/main/raft_kvserver/rpc.pb.cc.o
rpc: CMakeFiles/rpc.dir/src/fiber/fiber.cpp.o
rpc: CMakeFiles/rpc.dir/src/http/http_common.cpp.o
rpc: CMakeFiles/rpc.dir/src/http/request_parser.cpp.o
rpc: CMakeFiles/rpc.dir/src/locker/locker.cpp.o
rpc: CMakeFiles/rpc.dir/src/logger/logger.cpp.o
rpc: CMakeFiles/rpc.dir/src/net/epoll.cpp.o
rpc: CMakeFiles/rpc.dir/src/net/rpc/rpc_framework.pb.cc.o
rpc: CMakeFiles/rpc.dir/src/raft/raft_core.cpp.o
rpc: CMakeFiles/rpc.dir/src/raft/raft_core_proto.pb.cc.o
rpc: CMakeFiles/rpc.dir/src/thread/thread.cpp.o
rpc: CMakeFiles/rpc.dir/build.make
rpc: /usr/local/lib/libprotobuf.so
rpc: /usr/lib64/libssl.so
rpc: /usr/lib64/libcrypto.so
rpc: CMakeFiles/rpc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable rpc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rpc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rpc.dir/build: rpc
.PHONY : CMakeFiles/rpc.dir/build

CMakeFiles/rpc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rpc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rpc.dir/clean

CMakeFiles/rpc.dir/depend:
	cd /tmp/tmp.hmUOgZAfsx/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.hmUOgZAfsx /tmp/tmp.hmUOgZAfsx /tmp/tmp.hmUOgZAfsx/cmake-build-debug /tmp/tmp.hmUOgZAfsx/cmake-build-debug /tmp/tmp.hmUOgZAfsx/cmake-build-debug/CMakeFiles/rpc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rpc.dir/depend

