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
CMAKE_BINARY_DIR = /root/build/cpp_High_Performance_Server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test_timer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_timer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_timer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_timer.dir/flags.make

CMakeFiles/test_timer.dir/test/test_timer.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/test/test_timer.cpp.o: /root/build/cpp_High_Performance_Server/test/test_timer.cpp
CMakeFiles/test_timer.dir/test/test_timer.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_timer.dir/test/test_timer.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/test/test_timer.cpp.o -MF CMakeFiles/test_timer.dir/test/test_timer.cpp.o.d -o CMakeFiles/test_timer.dir/test/test_timer.cpp.o -c /root/build/cpp_High_Performance_Server/test/test_timer.cpp

CMakeFiles/test_timer.dir/test/test_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/test/test_timer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/test/test_timer.cpp > CMakeFiles/test_timer.dir/test/test_timer.cpp.i

CMakeFiles/test_timer.dir/test/test_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/test/test_timer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/test/test_timer.cpp -o CMakeFiles/test_timer.dir/test/test_timer.cpp.s

CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o: /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp
CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o -MF CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o.d -o CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o -c /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp

CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp > CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.i

CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/fiber/fiber.cpp -o CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.s

CMakeFiles/test_timer.dir/src/http/http_common.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/http/http_common.cpp.o: /root/build/cpp_High_Performance_Server/src/http/http_common.cpp
CMakeFiles/test_timer.dir/src/http/http_common.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_timer.dir/src/http/http_common.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/http/http_common.cpp.o -MF CMakeFiles/test_timer.dir/src/http/http_common.cpp.o.d -o CMakeFiles/test_timer.dir/src/http/http_common.cpp.o -c /root/build/cpp_High_Performance_Server/src/http/http_common.cpp

CMakeFiles/test_timer.dir/src/http/http_common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/http/http_common.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/http/http_common.cpp > CMakeFiles/test_timer.dir/src/http/http_common.cpp.i

CMakeFiles/test_timer.dir/src/http/http_common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/http/http_common.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/http/http_common.cpp -o CMakeFiles/test_timer.dir/src/http/http_common.cpp.s

CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o: /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp
CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o -MF CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o.d -o CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o -c /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp

CMakeFiles/test_timer.dir/src/http/request_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/http/request_parser.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp > CMakeFiles/test_timer.dir/src/http/request_parser.cpp.i

CMakeFiles/test_timer.dir/src/http/request_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/http/request_parser.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/http/request_parser.cpp -o CMakeFiles/test_timer.dir/src/http/request_parser.cpp.s

CMakeFiles/test_timer.dir/src/locker/locker.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/locker/locker.cpp.o: /root/build/cpp_High_Performance_Server/src/locker/locker.cpp
CMakeFiles/test_timer.dir/src/locker/locker.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_timer.dir/src/locker/locker.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/locker/locker.cpp.o -MF CMakeFiles/test_timer.dir/src/locker/locker.cpp.o.d -o CMakeFiles/test_timer.dir/src/locker/locker.cpp.o -c /root/build/cpp_High_Performance_Server/src/locker/locker.cpp

CMakeFiles/test_timer.dir/src/locker/locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/locker/locker.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/locker/locker.cpp > CMakeFiles/test_timer.dir/src/locker/locker.cpp.i

CMakeFiles/test_timer.dir/src/locker/locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/locker/locker.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/locker/locker.cpp -o CMakeFiles/test_timer.dir/src/locker/locker.cpp.s

CMakeFiles/test_timer.dir/src/logger/logger.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/logger/logger.cpp.o: /root/build/cpp_High_Performance_Server/src/logger/logger.cpp
CMakeFiles/test_timer.dir/src/logger/logger.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_timer.dir/src/logger/logger.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/logger/logger.cpp.o -MF CMakeFiles/test_timer.dir/src/logger/logger.cpp.o.d -o CMakeFiles/test_timer.dir/src/logger/logger.cpp.o -c /root/build/cpp_High_Performance_Server/src/logger/logger.cpp

CMakeFiles/test_timer.dir/src/logger/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/logger/logger.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/logger/logger.cpp > CMakeFiles/test_timer.dir/src/logger/logger.cpp.i

CMakeFiles/test_timer.dir/src/logger/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/logger/logger.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/logger/logger.cpp -o CMakeFiles/test_timer.dir/src/logger/logger.cpp.s

CMakeFiles/test_timer.dir/src/net/epoll.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/net/epoll.cpp.o: /root/build/cpp_High_Performance_Server/src/net/epoll.cpp
CMakeFiles/test_timer.dir/src/net/epoll.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test_timer.dir/src/net/epoll.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/net/epoll.cpp.o -MF CMakeFiles/test_timer.dir/src/net/epoll.cpp.o.d -o CMakeFiles/test_timer.dir/src/net/epoll.cpp.o -c /root/build/cpp_High_Performance_Server/src/net/epoll.cpp

CMakeFiles/test_timer.dir/src/net/epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/net/epoll.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/net/epoll.cpp > CMakeFiles/test_timer.dir/src/net/epoll.cpp.i

CMakeFiles/test_timer.dir/src/net/epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/net/epoll.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/net/epoll.cpp -o CMakeFiles/test_timer.dir/src/net/epoll.cpp.s

CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o: /root/build/cpp_High_Performance_Server/src/protobuf/test_proto.pb.cc
CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o -MF CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o.d -o CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o -c /root/build/cpp_High_Performance_Server/src/protobuf/test_proto.pb.cc

CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/protobuf/test_proto.pb.cc > CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.i

CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/protobuf/test_proto.pb.cc -o CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.s

CMakeFiles/test_timer.dir/src/thread/thread.cpp.o: CMakeFiles/test_timer.dir/flags.make
CMakeFiles/test_timer.dir/src/thread/thread.cpp.o: /root/build/cpp_High_Performance_Server/src/thread/thread.cpp
CMakeFiles/test_timer.dir/src/thread/thread.cpp.o: CMakeFiles/test_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/test_timer.dir/src/thread/thread.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_timer.dir/src/thread/thread.cpp.o -MF CMakeFiles/test_timer.dir/src/thread/thread.cpp.o.d -o CMakeFiles/test_timer.dir/src/thread/thread.cpp.o -c /root/build/cpp_High_Performance_Server/src/thread/thread.cpp

CMakeFiles/test_timer.dir/src/thread/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_timer.dir/src/thread/thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/build/cpp_High_Performance_Server/src/thread/thread.cpp > CMakeFiles/test_timer.dir/src/thread/thread.cpp.i

CMakeFiles/test_timer.dir/src/thread/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_timer.dir/src/thread/thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/build/cpp_High_Performance_Server/src/thread/thread.cpp -o CMakeFiles/test_timer.dir/src/thread/thread.cpp.s

# Object files for target test_timer
test_timer_OBJECTS = \
"CMakeFiles/test_timer.dir/test/test_timer.cpp.o" \
"CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o" \
"CMakeFiles/test_timer.dir/src/http/http_common.cpp.o" \
"CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o" \
"CMakeFiles/test_timer.dir/src/locker/locker.cpp.o" \
"CMakeFiles/test_timer.dir/src/logger/logger.cpp.o" \
"CMakeFiles/test_timer.dir/src/net/epoll.cpp.o" \
"CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o" \
"CMakeFiles/test_timer.dir/src/thread/thread.cpp.o"

# External object files for target test_timer
test_timer_EXTERNAL_OBJECTS =

/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/test/test_timer.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/fiber/fiber.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/http/http_common.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/http/request_parser.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/locker/locker.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/logger/logger.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/net/epoll.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/protobuf/test_proto.pb.cc.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/src/thread/thread.cpp.o
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/build.make
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: /usr/local/lib/libprotobuf.so
/root/build/cpp_High_Performance_Server/bin/Debug/test_timer: CMakeFiles/test_timer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable /root/build/cpp_High_Performance_Server/bin/Debug/test_timer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_timer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_timer.dir/build: /root/build/cpp_High_Performance_Server/bin/Debug/test_timer
.PHONY : CMakeFiles/test_timer.dir/build

CMakeFiles/test_timer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_timer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_timer.dir/clean

CMakeFiles/test_timer.dir/depend:
	cd /root/build/cpp_High_Performance_Server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/build/cpp_High_Performance_Server /root/build/cpp_High_Performance_Server /root/build/cpp_High_Performance_Server/cmake-build-debug /root/build/cpp_High_Performance_Server/cmake-build-debug /root/build/cpp_High_Performance_Server/cmake-build-debug/CMakeFiles/test_timer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_timer.dir/depend

