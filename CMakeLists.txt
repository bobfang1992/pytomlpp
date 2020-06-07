cmake_minimum_required(VERSION 3.9.0)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_CXX_STANDARD 17)


project(pytomlpp VERSION 0.1.0)
include_directories(src)
include_directories(third_party)
add_subdirectory(pybind11)
add_subdirectory(tests)

# main project
pybind11_add_module(pytomlpp src/pytomlpp.cpp)

# tests
enable_testing()

add_executable(pytomlpp-test 
    tests/test_lib_config.cpp)
target_link_libraries(pytomlpp-test gtest gmock gtest_main)

add_test(pytomlpp-test pytomlpp-test)