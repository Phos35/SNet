# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/projects/web_server/SNet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/projects/web_server/SNet/build

# Include any dependencies generated for this target.
include CMakeFiles/TEST.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TEST.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TEST.dir/flags.make

CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.o: ../tests/event/test_event_loop_thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.o -c /home/ubuntu/projects/web_server/SNet/tests/event/test_event_loop_thread.cpp

CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/tests/event/test_event_loop_thread.cpp > CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.i

CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/tests/event/test_event_loop_thread.cpp -o CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.s

CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.o: ../utils/buffer/buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/buffer/buffer.cpp

CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/buffer/buffer.cpp > CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.i

CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/buffer/buffer.cpp -o CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.s

CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.o: ../utils/buffer/byte_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/buffer/byte_buffer.cpp

CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/buffer/byte_buffer.cpp > CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.i

CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/buffer/byte_buffer.cpp -o CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.s

CMakeFiles/TEST.dir/utils/config/config.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/config/config.cpp.o: ../utils/config/config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TEST.dir/utils/config/config.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/config/config.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/config/config.cpp

CMakeFiles/TEST.dir/utils/config/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/config/config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/config/config.cpp > CMakeFiles/TEST.dir/utils/config/config.cpp.i

CMakeFiles/TEST.dir/utils/config/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/config/config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/config/config.cpp -o CMakeFiles/TEST.dir/utils/config/config.cpp.s

CMakeFiles/TEST.dir/utils/log/append_file.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/log/append_file.cpp.o: ../utils/log/append_file.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TEST.dir/utils/log/append_file.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/log/append_file.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/log/append_file.cpp

CMakeFiles/TEST.dir/utils/log/append_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/log/append_file.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/log/append_file.cpp > CMakeFiles/TEST.dir/utils/log/append_file.cpp.i

CMakeFiles/TEST.dir/utils/log/append_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/log/append_file.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/log/append_file.cpp -o CMakeFiles/TEST.dir/utils/log/append_file.cpp.s

CMakeFiles/TEST.dir/utils/log/async_log.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/log/async_log.cpp.o: ../utils/log/async_log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TEST.dir/utils/log/async_log.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/log/async_log.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/log/async_log.cpp

CMakeFiles/TEST.dir/utils/log/async_log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/log/async_log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/log/async_log.cpp > CMakeFiles/TEST.dir/utils/log/async_log.cpp.i

CMakeFiles/TEST.dir/utils/log/async_log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/log/async_log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/log/async_log.cpp -o CMakeFiles/TEST.dir/utils/log/async_log.cpp.s

CMakeFiles/TEST.dir/utils/log/log_file.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/log/log_file.cpp.o: ../utils/log/log_file.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TEST.dir/utils/log/log_file.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/log/log_file.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/log/log_file.cpp

CMakeFiles/TEST.dir/utils/log/log_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/log/log_file.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/log/log_file.cpp > CMakeFiles/TEST.dir/utils/log/log_file.cpp.i

CMakeFiles/TEST.dir/utils/log/log_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/log/log_file.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/log/log_file.cpp -o CMakeFiles/TEST.dir/utils/log/log_file.cpp.s

CMakeFiles/TEST.dir/utils/log/log_stream.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/log/log_stream.cpp.o: ../utils/log/log_stream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TEST.dir/utils/log/log_stream.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/log/log_stream.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/log/log_stream.cpp

CMakeFiles/TEST.dir/utils/log/log_stream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/log/log_stream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/log/log_stream.cpp > CMakeFiles/TEST.dir/utils/log/log_stream.cpp.i

CMakeFiles/TEST.dir/utils/log/log_stream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/log/log_stream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/log/log_stream.cpp -o CMakeFiles/TEST.dir/utils/log/log_stream.cpp.s

CMakeFiles/TEST.dir/utils/log/logger.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/log/logger.cpp.o: ../utils/log/logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TEST.dir/utils/log/logger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/log/logger.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/log/logger.cpp

CMakeFiles/TEST.dir/utils/log/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/log/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/log/logger.cpp > CMakeFiles/TEST.dir/utils/log/logger.cpp.i

CMakeFiles/TEST.dir/utils/log/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/log/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/log/logger.cpp -o CMakeFiles/TEST.dir/utils/log/logger.cpp.s

CMakeFiles/TEST.dir/utils/time/timestamp.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/utils/time/timestamp.cpp.o: ../utils/time/timestamp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/TEST.dir/utils/time/timestamp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/utils/time/timestamp.cpp.o -c /home/ubuntu/projects/web_server/SNet/utils/time/timestamp.cpp

CMakeFiles/TEST.dir/utils/time/timestamp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/utils/time/timestamp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/utils/time/timestamp.cpp > CMakeFiles/TEST.dir/utils/time/timestamp.cpp.i

CMakeFiles/TEST.dir/utils/time/timestamp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/utils/time/timestamp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/utils/time/timestamp.cpp -o CMakeFiles/TEST.dir/utils/time/timestamp.cpp.s

CMakeFiles/TEST.dir/event/epoller.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/event/epoller.cpp.o: ../event/epoller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/TEST.dir/event/epoller.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/event/epoller.cpp.o -c /home/ubuntu/projects/web_server/SNet/event/epoller.cpp

CMakeFiles/TEST.dir/event/epoller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/event/epoller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/event/epoller.cpp > CMakeFiles/TEST.dir/event/epoller.cpp.i

CMakeFiles/TEST.dir/event/epoller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/event/epoller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/event/epoller.cpp -o CMakeFiles/TEST.dir/event/epoller.cpp.s

CMakeFiles/TEST.dir/event/event.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/event/event.cpp.o: ../event/event.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/TEST.dir/event/event.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/event/event.cpp.o -c /home/ubuntu/projects/web_server/SNet/event/event.cpp

CMakeFiles/TEST.dir/event/event.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/event/event.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/event/event.cpp > CMakeFiles/TEST.dir/event/event.cpp.i

CMakeFiles/TEST.dir/event/event.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/event/event.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/event/event.cpp -o CMakeFiles/TEST.dir/event/event.cpp.s

CMakeFiles/TEST.dir/event/event_loop.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/event/event_loop.cpp.o: ../event/event_loop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/TEST.dir/event/event_loop.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/event/event_loop.cpp.o -c /home/ubuntu/projects/web_server/SNet/event/event_loop.cpp

CMakeFiles/TEST.dir/event/event_loop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/event/event_loop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/event/event_loop.cpp > CMakeFiles/TEST.dir/event/event_loop.cpp.i

CMakeFiles/TEST.dir/event/event_loop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/event/event_loop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/event/event_loop.cpp -o CMakeFiles/TEST.dir/event/event_loop.cpp.s

CMakeFiles/TEST.dir/event/event_loop_thread.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/event/event_loop_thread.cpp.o: ../event/event_loop_thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/TEST.dir/event/event_loop_thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/event/event_loop_thread.cpp.o -c /home/ubuntu/projects/web_server/SNet/event/event_loop_thread.cpp

CMakeFiles/TEST.dir/event/event_loop_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/event/event_loop_thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/event/event_loop_thread.cpp > CMakeFiles/TEST.dir/event/event_loop_thread.cpp.i

CMakeFiles/TEST.dir/event/event_loop_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/event/event_loop_thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/event/event_loop_thread.cpp -o CMakeFiles/TEST.dir/event/event_loop_thread.cpp.s

CMakeFiles/TEST.dir/event/iomutiplexing.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/event/iomutiplexing.cpp.o: ../event/iomutiplexing.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/TEST.dir/event/iomutiplexing.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/event/iomutiplexing.cpp.o -c /home/ubuntu/projects/web_server/SNet/event/iomutiplexing.cpp

CMakeFiles/TEST.dir/event/iomutiplexing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/event/iomutiplexing.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/projects/web_server/SNet/event/iomutiplexing.cpp > CMakeFiles/TEST.dir/event/iomutiplexing.cpp.i

CMakeFiles/TEST.dir/event/iomutiplexing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/event/iomutiplexing.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/projects/web_server/SNet/event/iomutiplexing.cpp -o CMakeFiles/TEST.dir/event/iomutiplexing.cpp.s

# Object files for target TEST
TEST_OBJECTS = \
"CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.o" \
"CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.o" \
"CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.o" \
"CMakeFiles/TEST.dir/utils/config/config.cpp.o" \
"CMakeFiles/TEST.dir/utils/log/append_file.cpp.o" \
"CMakeFiles/TEST.dir/utils/log/async_log.cpp.o" \
"CMakeFiles/TEST.dir/utils/log/log_file.cpp.o" \
"CMakeFiles/TEST.dir/utils/log/log_stream.cpp.o" \
"CMakeFiles/TEST.dir/utils/log/logger.cpp.o" \
"CMakeFiles/TEST.dir/utils/time/timestamp.cpp.o" \
"CMakeFiles/TEST.dir/event/epoller.cpp.o" \
"CMakeFiles/TEST.dir/event/event.cpp.o" \
"CMakeFiles/TEST.dir/event/event_loop.cpp.o" \
"CMakeFiles/TEST.dir/event/event_loop_thread.cpp.o" \
"CMakeFiles/TEST.dir/event/iomutiplexing.cpp.o"

# External object files for target TEST
TEST_EXTERNAL_OBJECTS =

TEST: CMakeFiles/TEST.dir/tests/event/test_event_loop_thread.cpp.o
TEST: CMakeFiles/TEST.dir/utils/buffer/buffer.cpp.o
TEST: CMakeFiles/TEST.dir/utils/buffer/byte_buffer.cpp.o
TEST: CMakeFiles/TEST.dir/utils/config/config.cpp.o
TEST: CMakeFiles/TEST.dir/utils/log/append_file.cpp.o
TEST: CMakeFiles/TEST.dir/utils/log/async_log.cpp.o
TEST: CMakeFiles/TEST.dir/utils/log/log_file.cpp.o
TEST: CMakeFiles/TEST.dir/utils/log/log_stream.cpp.o
TEST: CMakeFiles/TEST.dir/utils/log/logger.cpp.o
TEST: CMakeFiles/TEST.dir/utils/time/timestamp.cpp.o
TEST: CMakeFiles/TEST.dir/event/epoller.cpp.o
TEST: CMakeFiles/TEST.dir/event/event.cpp.o
TEST: CMakeFiles/TEST.dir/event/event_loop.cpp.o
TEST: CMakeFiles/TEST.dir/event/event_loop_thread.cpp.o
TEST: CMakeFiles/TEST.dir/event/iomutiplexing.cpp.o
TEST: CMakeFiles/TEST.dir/build.make
TEST: CMakeFiles/TEST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/projects/web_server/SNet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable TEST"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TEST.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TEST.dir/build: TEST

.PHONY : CMakeFiles/TEST.dir/build

CMakeFiles/TEST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TEST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TEST.dir/clean

CMakeFiles/TEST.dir/depend:
	cd /home/ubuntu/projects/web_server/SNet/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/projects/web_server/SNet /home/ubuntu/projects/web_server/SNet /home/ubuntu/projects/web_server/SNet/build /home/ubuntu/projects/web_server/SNet/build /home/ubuntu/projects/web_server/SNet/build/CMakeFiles/TEST.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TEST.dir/depend

