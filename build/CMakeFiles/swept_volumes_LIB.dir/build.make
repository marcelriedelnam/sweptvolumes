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
CMAKE_SOURCE_DIR = "/share/BA/Swept Volumes"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/share/BA/Swept Volumes/build"

# Include any dependencies generated for this target.
include CMakeFiles/swept_volumes_LIB.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/swept_volumes_LIB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/swept_volumes_LIB.dir/flags.make

CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.o: CMakeFiles/swept_volumes_LIB.dir/flags.make
CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.o: ../src/csv_reader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/share/BA/Swept Volumes/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.o -c "/share/BA/Swept Volumes/src/csv_reader.cpp"

CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/share/BA/Swept Volumes/src/csv_reader.cpp" > CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.i

CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/share/BA/Swept Volumes/src/csv_reader.cpp" -o CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.s

CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.o: CMakeFiles/swept_volumes_LIB.dir/flags.make
CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/share/BA/Swept Volumes/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.o -c "/share/BA/Swept Volumes/src/main.cpp"

CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/share/BA/Swept Volumes/src/main.cpp" > CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.i

CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/share/BA/Swept Volumes/src/main.cpp" -o CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.s

CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.o: CMakeFiles/swept_volumes_LIB.dir/flags.make
CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.o: ../src/obj_reader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/share/BA/Swept Volumes/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.o -c "/share/BA/Swept Volumes/src/obj_reader.cpp"

CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/share/BA/Swept Volumes/src/obj_reader.cpp" > CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.i

CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/share/BA/Swept Volumes/src/obj_reader.cpp" -o CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.s

CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.o: CMakeFiles/swept_volumes_LIB.dir/flags.make
CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.o: ../src/transform.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/share/BA/Swept Volumes/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.o -c "/share/BA/Swept Volumes/src/transform.cpp"

CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/share/BA/Swept Volumes/src/transform.cpp" > CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.i

CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/share/BA/Swept Volumes/src/transform.cpp" -o CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.s

CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.o: CMakeFiles/swept_volumes_LIB.dir/flags.make
CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.o: ../src/write_file.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/share/BA/Swept Volumes/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.o -c "/share/BA/Swept Volumes/src/write_file.cpp"

CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/share/BA/Swept Volumes/src/write_file.cpp" > CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.i

CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/share/BA/Swept Volumes/src/write_file.cpp" -o CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.s

# Object files for target swept_volumes_LIB
swept_volumes_LIB_OBJECTS = \
"CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.o" \
"CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.o" \
"CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.o" \
"CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.o" \
"CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.o"

# External object files for target swept_volumes_LIB
swept_volumes_LIB_EXTERNAL_OBJECTS =

libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/src/csv_reader.cpp.o
libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/src/main.cpp.o
libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/src/obj_reader.cpp.o
libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/src/transform.cpp.o
libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/src/write_file.cpp.o
libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/build.make
libswept_volumes_LIB.so: CMakeFiles/swept_volumes_LIB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/share/BA/Swept Volumes/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library libswept_volumes_LIB.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/swept_volumes_LIB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/swept_volumes_LIB.dir/build: libswept_volumes_LIB.so

.PHONY : CMakeFiles/swept_volumes_LIB.dir/build

CMakeFiles/swept_volumes_LIB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/swept_volumes_LIB.dir/cmake_clean.cmake
.PHONY : CMakeFiles/swept_volumes_LIB.dir/clean

CMakeFiles/swept_volumes_LIB.dir/depend:
	cd "/share/BA/Swept Volumes/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/share/BA/Swept Volumes" "/share/BA/Swept Volumes" "/share/BA/Swept Volumes/build" "/share/BA/Swept Volumes/build" "/share/BA/Swept Volumes/build/CMakeFiles/swept_volumes_LIB.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/swept_volumes_LIB.dir/depend

