# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/austin/program/chromatags/realtime

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/austin/program/chromatags/realtime/build

# Include any dependencies generated for this target.
include CMakeFiles/chromatag.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chromatag.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chromatag.dir/flags.make

CMakeFiles/chromatag.dir/chromatag.cpp.o: CMakeFiles/chromatag.dir/flags.make
CMakeFiles/chromatag.dir/chromatag.cpp.o: ../chromatag.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/austin/program/chromatags/realtime/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/chromatag.dir/chromatag.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/chromatag.dir/chromatag.cpp.o -c /Users/austin/program/chromatags/realtime/chromatag.cpp

CMakeFiles/chromatag.dir/chromatag.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chromatag.dir/chromatag.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/austin/program/chromatags/realtime/chromatag.cpp > CMakeFiles/chromatag.dir/chromatag.cpp.i

CMakeFiles/chromatag.dir/chromatag.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chromatag.dir/chromatag.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/austin/program/chromatags/realtime/chromatag.cpp -o CMakeFiles/chromatag.dir/chromatag.cpp.s

CMakeFiles/chromatag.dir/chromatag.cpp.o.requires:
.PHONY : CMakeFiles/chromatag.dir/chromatag.cpp.o.requires

CMakeFiles/chromatag.dir/chromatag.cpp.o.provides: CMakeFiles/chromatag.dir/chromatag.cpp.o.requires
	$(MAKE) -f CMakeFiles/chromatag.dir/build.make CMakeFiles/chromatag.dir/chromatag.cpp.o.provides.build
.PHONY : CMakeFiles/chromatag.dir/chromatag.cpp.o.provides

CMakeFiles/chromatag.dir/chromatag.cpp.o.provides.build: CMakeFiles/chromatag.dir/chromatag.cpp.o

# Object files for target chromatag
chromatag_OBJECTS = \
"CMakeFiles/chromatag.dir/chromatag.cpp.o"

# External object files for target chromatag
chromatag_EXTERNAL_OBJECTS =

chromatag: CMakeFiles/chromatag.dir/chromatag.cpp.o
chromatag: CMakeFiles/chromatag.dir/build.make
chromatag: /opt/local/lib/libopencv_videostab.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_video.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_ts.a
chromatag: /opt/local/lib/libopencv_superres.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_stitching.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_photo.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_objdetect.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_nonfree.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_ml.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_legacy.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_imgproc.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_highgui.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_gpu.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_flann.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_features2d.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_core.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_contrib.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_calib3d.2.4.10.dylib
chromatag: ../libapriltag.a
chromatag: /opt/local/lib/libopencv_nonfree.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_gpu.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_photo.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_objdetect.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_legacy.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_video.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_ml.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_calib3d.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_features2d.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_highgui.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_imgproc.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_flann.2.4.10.dylib
chromatag: /opt/local/lib/libopencv_core.2.4.10.dylib
chromatag: CMakeFiles/chromatag.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable chromatag"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chromatag.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chromatag.dir/build: chromatag
.PHONY : CMakeFiles/chromatag.dir/build

CMakeFiles/chromatag.dir/requires: CMakeFiles/chromatag.dir/chromatag.cpp.o.requires
.PHONY : CMakeFiles/chromatag.dir/requires

CMakeFiles/chromatag.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chromatag.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chromatag.dir/clean

CMakeFiles/chromatag.dir/depend:
	cd /Users/austin/program/chromatags/realtime/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/austin/program/chromatags/realtime /Users/austin/program/chromatags/realtime /Users/austin/program/chromatags/realtime/build /Users/austin/program/chromatags/realtime/build /Users/austin/program/chromatags/realtime/build/CMakeFiles/chromatag.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chromatag.dir/depend

