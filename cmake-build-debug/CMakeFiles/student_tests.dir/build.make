# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\fran_\Documents\C++ for Complete Beginners\P5"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles\student_tests.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\student_tests.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\student_tests.dir\flags.make

CMakeFiles\student_tests.dir\student_tests.cpp.obj: CMakeFiles\student_tests.dir\flags.make
CMakeFiles\student_tests.dir\student_tests.cpp.obj: ..\student_tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/student_tests.dir/student_tests.cpp.obj"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\student_tests.dir\student_tests.cpp.obj /FdCMakeFiles\student_tests.dir\ /FS -c "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\student_tests.cpp"
<<

CMakeFiles\student_tests.dir\student_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/student_tests.dir/student_tests.cpp.i"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe > CMakeFiles\student_tests.dir\student_tests.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\student_tests.cpp"
<<

CMakeFiles\student_tests.dir\student_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/student_tests.dir/student_tests.cpp.s"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\student_tests.dir\student_tests.cpp.s /c "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\student_tests.cpp"
<<

# Object files for target student_tests
student_tests_OBJECTS = \
"CMakeFiles\student_tests.dir\student_tests.cpp.obj"

# External object files for target student_tests
student_tests_EXTERNAL_OBJECTS =

student_tests.exe: CMakeFiles\student_tests.dir\student_tests.cpp.obj
student_tests.exe: CMakeFiles\student_tests.dir\build.make
student_tests.exe: CMakeFiles\student_tests.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable student_tests.exe"
	"C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\student_tests.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100162~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100162~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\student_tests.dir\objects1.rsp @<<
 /out:student_tests.exe /implib:student_tests.lib /pdb:"C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug\student_tests.pdb" /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\student_tests.dir\build: student_tests.exe

.PHONY : CMakeFiles\student_tests.dir\build

CMakeFiles\student_tests.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\student_tests.dir\cmake_clean.cmake
.PHONY : CMakeFiles\student_tests.dir\clean

CMakeFiles\student_tests.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" "C:\Users\fran_\Documents\C++ for Complete Beginners\P5" "C:\Users\fran_\Documents\C++ for Complete Beginners\P5" "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug" "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug" "C:\Users\fran_\Documents\C++ for Complete Beginners\P5\cmake-build-debug\CMakeFiles\student_tests.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles\student_tests.dir\depend

