# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/jgarcia/Downloads/clion-2017.2.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/jgarcia/Downloads/clion-2017.2.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jgarcia/projects/djsr/dj_mix_recorder_clion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/djsr_clion.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/djsr_clion.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/djsr_clion.dir/flags.make

CMakeFiles/djsr_clion.dir/main.c.o: CMakeFiles/djsr_clion.dir/flags.make
CMakeFiles/djsr_clion.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/djsr_clion.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/djsr_clion.dir/main.c.o   -c /home/jgarcia/projects/djsr/dj_mix_recorder_clion/main.c

CMakeFiles/djsr_clion.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/djsr_clion.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jgarcia/projects/djsr/dj_mix_recorder_clion/main.c > CMakeFiles/djsr_clion.dir/main.c.i

CMakeFiles/djsr_clion.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/djsr_clion.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jgarcia/projects/djsr/dj_mix_recorder_clion/main.c -o CMakeFiles/djsr_clion.dir/main.c.s

CMakeFiles/djsr_clion.dir/main.c.o.requires:

.PHONY : CMakeFiles/djsr_clion.dir/main.c.o.requires

CMakeFiles/djsr_clion.dir/main.c.o.provides: CMakeFiles/djsr_clion.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/djsr_clion.dir/build.make CMakeFiles/djsr_clion.dir/main.c.o.provides.build
.PHONY : CMakeFiles/djsr_clion.dir/main.c.o.provides

CMakeFiles/djsr_clion.dir/main.c.o.provides.build: CMakeFiles/djsr_clion.dir/main.c.o


CMakeFiles/djsr_clion.dir/APlay/APlay.c.o: CMakeFiles/djsr_clion.dir/flags.make
CMakeFiles/djsr_clion.dir/APlay/APlay.c.o: ../APlay/APlay.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/djsr_clion.dir/APlay/APlay.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/djsr_clion.dir/APlay/APlay.c.o   -c /home/jgarcia/projects/djsr/dj_mix_recorder_clion/APlay/APlay.c

CMakeFiles/djsr_clion.dir/APlay/APlay.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/djsr_clion.dir/APlay/APlay.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jgarcia/projects/djsr/dj_mix_recorder_clion/APlay/APlay.c > CMakeFiles/djsr_clion.dir/APlay/APlay.c.i

CMakeFiles/djsr_clion.dir/APlay/APlay.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/djsr_clion.dir/APlay/APlay.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jgarcia/projects/djsr/dj_mix_recorder_clion/APlay/APlay.c -o CMakeFiles/djsr_clion.dir/APlay/APlay.c.s

CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.requires:

.PHONY : CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.requires

CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.provides: CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.requires
	$(MAKE) -f CMakeFiles/djsr_clion.dir/build.make CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.provides.build
.PHONY : CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.provides

CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.provides.build: CMakeFiles/djsr_clion.dir/APlay/APlay.c.o


CMakeFiles/djsr_clion.dir/app/app.c.o: CMakeFiles/djsr_clion.dir/flags.make
CMakeFiles/djsr_clion.dir/app/app.c.o: ../app/app.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/djsr_clion.dir/app/app.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/djsr_clion.dir/app/app.c.o   -c /home/jgarcia/projects/djsr/dj_mix_recorder_clion/app/app.c

CMakeFiles/djsr_clion.dir/app/app.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/djsr_clion.dir/app/app.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jgarcia/projects/djsr/dj_mix_recorder_clion/app/app.c > CMakeFiles/djsr_clion.dir/app/app.c.i

CMakeFiles/djsr_clion.dir/app/app.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/djsr_clion.dir/app/app.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jgarcia/projects/djsr/dj_mix_recorder_clion/app/app.c -o CMakeFiles/djsr_clion.dir/app/app.c.s

CMakeFiles/djsr_clion.dir/app/app.c.o.requires:

.PHONY : CMakeFiles/djsr_clion.dir/app/app.c.o.requires

CMakeFiles/djsr_clion.dir/app/app.c.o.provides: CMakeFiles/djsr_clion.dir/app/app.c.o.requires
	$(MAKE) -f CMakeFiles/djsr_clion.dir/build.make CMakeFiles/djsr_clion.dir/app/app.c.o.provides.build
.PHONY : CMakeFiles/djsr_clion.dir/app/app.c.o.provides

CMakeFiles/djsr_clion.dir/app/app.c.o.provides.build: CMakeFiles/djsr_clion.dir/app/app.c.o


# Object files for target djsr_clion
djsr_clion_OBJECTS = \
"CMakeFiles/djsr_clion.dir/main.c.o" \
"CMakeFiles/djsr_clion.dir/APlay/APlay.c.o" \
"CMakeFiles/djsr_clion.dir/app/app.c.o"

# External object files for target djsr_clion
djsr_clion_EXTERNAL_OBJECTS =

djsr_clion: CMakeFiles/djsr_clion.dir/main.c.o
djsr_clion: CMakeFiles/djsr_clion.dir/APlay/APlay.c.o
djsr_clion: CMakeFiles/djsr_clion.dir/app/app.c.o
djsr_clion: CMakeFiles/djsr_clion.dir/build.make
djsr_clion: CMakeFiles/djsr_clion.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable djsr_clion"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/djsr_clion.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/djsr_clion.dir/build: djsr_clion

.PHONY : CMakeFiles/djsr_clion.dir/build

CMakeFiles/djsr_clion.dir/requires: CMakeFiles/djsr_clion.dir/main.c.o.requires
CMakeFiles/djsr_clion.dir/requires: CMakeFiles/djsr_clion.dir/APlay/APlay.c.o.requires
CMakeFiles/djsr_clion.dir/requires: CMakeFiles/djsr_clion.dir/app/app.c.o.requires

.PHONY : CMakeFiles/djsr_clion.dir/requires

CMakeFiles/djsr_clion.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/djsr_clion.dir/cmake_clean.cmake
.PHONY : CMakeFiles/djsr_clion.dir/clean

CMakeFiles/djsr_clion.dir/depend:
	cd /home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgarcia/projects/djsr/dj_mix_recorder_clion /home/jgarcia/projects/djsr/dj_mix_recorder_clion /home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug /home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug /home/jgarcia/projects/djsr/dj_mix_recorder_clion/cmake-build-debug/CMakeFiles/djsr_clion.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/djsr_clion.dir/depend

