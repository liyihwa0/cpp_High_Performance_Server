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
CMAKE_SOURCE_DIR = /root/source/cpp_High_Performance_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/source/cpp_High_Performance_Server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/spin_lock.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/spin_lock.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/spin_lock.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/spin_lock.dir/flags.make

CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o: /root/source/cpp_High_Performance_Server/test/std/spin_lock.cpp
CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o -MF CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o.d -o CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o -c /root/source/cpp_High_Performance_Server/test/std/spin_lock.cpp

CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/test/std/spin_lock.cpp > CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.i

CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/test/std/spin_lock.cpp -o CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.s

CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o: /root/source/cpp_High_Performance_Server/src/fiber/fiber.cpp
CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o -MF CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o.d -o CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o -c /root/source/cpp_High_Performance_Server/src/fiber/fiber.cpp

CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/fiber/fiber.cpp > CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.i

CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/fiber/fiber.cpp -o CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.s

CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o: /root/source/cpp_High_Performance_Server/src/http/http_common.cpp
CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o -MF CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o.d -o CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o -c /root/source/cpp_High_Performance_Server/src/http/http_common.cpp

CMakeFiles/spin_lock.dir/src/http/http_common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/http/http_common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/http/http_common.cpp > CMakeFiles/spin_lock.dir/src/http/http_common.cpp.i

CMakeFiles/spin_lock.dir/src/http/http_common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/http/http_common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/http/http_common.cpp -o CMakeFiles/spin_lock.dir/src/http/http_common.cpp.s

CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o: /root/source/cpp_High_Performance_Server/src/http/request_parser.cpp
CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o -MF CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o.d -o CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o -c /root/source/cpp_High_Performance_Server/src/http/request_parser.cpp

CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/http/request_parser.cpp > CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.i

CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/http/request_parser.cpp -o CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.s

CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o: /root/source/cpp_High_Performance_Server/src/locker/locker.cpp
CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o -MF CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o.d -o CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o -c /root/source/cpp_High_Performance_Server/src/locker/locker.cpp

CMakeFiles/spin_lock.dir/src/locker/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/locker/locker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/locker/locker.cpp > CMakeFiles/spin_lock.dir/src/locker/locker.cpp.i

CMakeFiles/spin_lock.dir/src/locker/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/locker/locker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/locker/locker.cpp -o CMakeFiles/spin_lock.dir/src/locker/locker.cpp.s

CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o: /root/source/cpp_High_Performance_Server/src/logger/logger.cpp
CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o -MF CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o.d -o CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o -c /root/source/cpp_High_Performance_Server/src/logger/logger.cpp

CMakeFiles/spin_lock.dir/src/logger/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/logger/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/logger/logger.cpp > CMakeFiles/spin_lock.dir/src/logger/logger.cpp.i

CMakeFiles/spin_lock.dir/src/logger/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/logger/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/logger/logger.cpp -o CMakeFiles/spin_lock.dir/src/logger/logger.cpp.s

CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o: /root/source/cpp_High_Performance_Server/src/net/epoll.cpp
CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o -MF CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o.d -o CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o -c /root/source/cpp_High_Performance_Server/src/net/epoll.cpp

CMakeFiles/spin_lock.dir/src/net/epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/net/epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/net/epoll.cpp > CMakeFiles/spin_lock.dir/src/net/epoll.cpp.i

CMakeFiles/spin_lock.dir/src/net/epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/net/epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/net/epoll.cpp -o CMakeFiles/spin_lock.dir/src/net/epoll.cpp.s

CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o: /root/source/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc
CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o -MF CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o.d -o CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o -c /root/source/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc

CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc > CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.i

CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/net/rpc/rpc_framework.pb.cc -o CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.s

CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o: /root/source/cpp_High_Performance_Server/src/raft/raft_core.cpp
CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o -MF CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o.d -o CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o -c /root/source/cpp_High_Performance_Server/src/raft/raft_core.cpp

CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/raft/raft_core.cpp > CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.i

CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/raft/raft_core.cpp -o CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.s

CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o: /root/source/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc
CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o -MF CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o.d -o CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o -c /root/source/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc

CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc > CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.i

CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/raft/raft_core_proto.pb.cc -o CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.s

CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o: CMakeFiles/spin_lock.dir/flags.make
CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o: /root/source/cpp_High_Performance_Server/src/thread/thread.cpp
CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o: CMakeFiles/spin_lock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o -MF CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o.d -o CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o -c /root/source/cpp_High_Performance_Server/src/thread/thread.cpp

CMakeFiles/spin_lock.dir/src/thread/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spin_lock.dir/src/thread/thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/source/cpp_High_Performance_Server/src/thread/thread.cpp > CMakeFiles/spin_lock.dir/src/thread/thread.cpp.i

CMakeFiles/spin_lock.dir/src/thread/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spin_lock.dir/src/thread/thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/source/cpp_High_Performance_Server/src/thread/thread.cpp -o CMakeFiles/spin_lock.dir/src/thread/thread.cpp.s

# Object files for target spin_lock
spin_lock_OBJECTS = \
"CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o" \
"CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o" \
"CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o" \
"CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o" \
"CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o" \
"CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o" \
"CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o" \
"CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o" \
"CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o" \
"CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o" \
"CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o"

# External object files for target spin_lock
spin_lock_EXTERNAL_OBJECTS =

spin_lock: CMakeFiles/spin_lock.dir/test/std/spin_lock.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/fiber/fiber.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/http/http_common.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/http/request_parser.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/locker/locker.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/logger/logger.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/net/epoll.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/net/rpc/rpc_framework.pb.cc.o
spin_lock: CMakeFiles/spin_lock.dir/src/raft/raft_core.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/src/raft/raft_core_proto.pb.cc.o
spin_lock: CMakeFiles/spin_lock.dir/src/thread/thread.cpp.o
spin_lock: CMakeFiles/spin_lock.dir/build.make
spin_lock: /usr/local/lib/libprotobuf.so
spin_lock: /usr/lib/x86_64-linux-gnu/libssl.so
spin_lock: /usr/lib/x86_64-linux-gnu/libcrypto.so
spin_lock: CMakeFiles/spin_lock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable spin_lock"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spin_lock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/spin_lock.dir/build: spin_lock
.PHONY : CMakeFiles/spin_lock.dir/build

CMakeFiles/spin_lock.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/spin_lock.dir/cmake_clean.cmake
.PHONY : CMakeFiles/spin_lock.dir/clean

CMakeFiles/spin_lock.dir/depend:
	cd /root/source/cpp_High_Performance_Server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/source/cpp_High_Performance_Server /root/source/cpp_High_Performance_Server /root/source/cpp_High_Performance_Server/cmake-build-debug /root/source/cpp_High_Performance_Server/cmake-build-debug /root/source/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles/spin_lock.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/spin_lock.dir/depend

