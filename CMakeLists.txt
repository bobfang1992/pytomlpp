cmake_minimum_required(VERSION 2.8.12)
cmake_policy(SET CMP0048 NEW)

# main project
project(pytomlpp VERSION 0.1.0)

include_directories(include)
include_directories(thirdparty)
add_subdirectory(pybind11)
pybind11_add_module(pytomlpp src/pytomlpp.cpp)

# tests

enable_testing()
add_subdirectory(tests)
add_executable(pytomlpp-test 
    tests/test_example.cpp 
    tests/test_example2.cpp)
target_link_libraries(pytomlpp-test gtest gmock gtest_main)
add_test(pytomlpp-test pytomlpp-test)