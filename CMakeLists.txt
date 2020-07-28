cmake_minimum_required(VERSION 3.5)
project(instructor_tests CXX)

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE student_tests.cpp hash.hpp merkle_tree.hpp)

# create unittests
add_executable(student_tests catch.hpp ${SOURCE})
set_target_properties(student_tests PROPERTIES LINKER_LANGUAGE CXX)

enable_testing()

# unit tests
add_test(unittest student_tests)

     
