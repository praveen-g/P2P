# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /home/praveen/Downloads/clion-2017.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/praveen/Downloads/clion-2017.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/praveen/Downloads/sample code"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/praveen/Downloads/sample code/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/p2papp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/p2papp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/p2papp.dir/flags.make

CMakeFiles/p2papp.dir/main.cc.o: CMakeFiles/p2papp.dir/flags.make
CMakeFiles/p2papp.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/praveen/Downloads/sample code/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/p2papp.dir/main.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2papp.dir/main.cc.o -c "/home/praveen/Downloads/sample code/main.cc"

CMakeFiles/p2papp.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2papp.dir/main.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/praveen/Downloads/sample code/main.cc" > CMakeFiles/p2papp.dir/main.cc.i

CMakeFiles/p2papp.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2papp.dir/main.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/praveen/Downloads/sample code/main.cc" -o CMakeFiles/p2papp.dir/main.cc.s

CMakeFiles/p2papp.dir/main.cc.o.requires:

.PHONY : CMakeFiles/p2papp.dir/main.cc.o.requires

CMakeFiles/p2papp.dir/main.cc.o.provides: CMakeFiles/p2papp.dir/main.cc.o.requires
	$(MAKE) -f CMakeFiles/p2papp.dir/build.make CMakeFiles/p2papp.dir/main.cc.o.provides.build
.PHONY : CMakeFiles/p2papp.dir/main.cc.o.provides

CMakeFiles/p2papp.dir/main.cc.o.provides.build: CMakeFiles/p2papp.dir/main.cc.o


CMakeFiles/p2papp.dir/moc_main.cpp.o: CMakeFiles/p2papp.dir/flags.make
CMakeFiles/p2papp.dir/moc_main.cpp.o: ../moc_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/praveen/Downloads/sample code/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/p2papp.dir/moc_main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p2papp.dir/moc_main.cpp.o -c "/home/praveen/Downloads/sample code/moc_main.cpp"

CMakeFiles/p2papp.dir/moc_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p2papp.dir/moc_main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/praveen/Downloads/sample code/moc_main.cpp" > CMakeFiles/p2papp.dir/moc_main.cpp.i

CMakeFiles/p2papp.dir/moc_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p2papp.dir/moc_main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/praveen/Downloads/sample code/moc_main.cpp" -o CMakeFiles/p2papp.dir/moc_main.cpp.s

CMakeFiles/p2papp.dir/moc_main.cpp.o.requires:

.PHONY : CMakeFiles/p2papp.dir/moc_main.cpp.o.requires

CMakeFiles/p2papp.dir/moc_main.cpp.o.provides: CMakeFiles/p2papp.dir/moc_main.cpp.o.requires
	$(MAKE) -f CMakeFiles/p2papp.dir/build.make CMakeFiles/p2papp.dir/moc_main.cpp.o.provides.build
.PHONY : CMakeFiles/p2papp.dir/moc_main.cpp.o.provides

CMakeFiles/p2papp.dir/moc_main.cpp.o.provides.build: CMakeFiles/p2papp.dir/moc_main.cpp.o


# Object files for target p2papp
p2papp_OBJECTS = \
"CMakeFiles/p2papp.dir/main.cc.o" \
"CMakeFiles/p2papp.dir/moc_main.cpp.o"

# External object files for target p2papp
p2papp_EXTERNAL_OBJECTS =

p2papp: CMakeFiles/p2papp.dir/main.cc.o
p2papp: CMakeFiles/p2papp.dir/moc_main.cpp.o
p2papp: CMakeFiles/p2papp.dir/build.make
p2papp: CMakeFiles/p2papp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/praveen/Downloads/sample code/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable p2papp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/p2papp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/p2papp.dir/build: p2papp

.PHONY : CMakeFiles/p2papp.dir/build

CMakeFiles/p2papp.dir/requires: CMakeFiles/p2papp.dir/main.cc.o.requires
CMakeFiles/p2papp.dir/requires: CMakeFiles/p2papp.dir/moc_main.cpp.o.requires

.PHONY : CMakeFiles/p2papp.dir/requires

CMakeFiles/p2papp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/p2papp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/p2papp.dir/clean

CMakeFiles/p2papp.dir/depend:
	cd "/home/praveen/Downloads/sample code/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/praveen/Downloads/sample code" "/home/praveen/Downloads/sample code" "/home/praveen/Downloads/sample code/cmake-build-debug" "/home/praveen/Downloads/sample code/cmake-build-debug" "/home/praveen/Downloads/sample code/cmake-build-debug/CMakeFiles/p2papp.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/p2papp.dir/depend

