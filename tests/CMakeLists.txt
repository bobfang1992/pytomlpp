cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(
  pytomlpp-test
  LANGUAGES CXX
)

get_filename_component(PYTOMLPP_ROOT ../ ABSOLUTE)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# ---- Dependencies ----

include("${PYTOMLPP_ROOT}/cmake/CPM.cmake")

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.8.1
  VERSION 1.8.1
  OPTIONS
      "INSTALL_GTEST OFF"
      "gtest_force_shared_crt ON"
)

CPMAddPackage(
  NAME tomlplusplus
  GITHUB_REPOSITORY marzer/tomlplusplus
  VERSION 1.3.0
  DOWNLOAD_ONLY True
)
add_library(tomlplusplus INTERFACE IMPORTED)
target_compile_features(tomlplusplus INTERFACE cxx_std_17)
target_include_directories(tomlplusplus INTERFACE "${tomlplusplus_SOURCE_DIR}/include")

# The following code makes sure toml-test is checked out automatically without manual intervention from user
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# ---- Create binary ----

add_executable(pytomlpp-test "${CMAKE_CURRENT_SOURCE_DIR}/test_lib_config.cpp")
target_link_libraries(pytomlpp-test tomlplusplus gtest gmock gtest_main)
set_target_properties(pytomlpp-test PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if (NOT TEST_INSTALLED_VERSION)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(pytomlpp-test PUBLIC -Wall -pedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(pytomlpp-test PUBLIC /W4 /WX)
  endif()
endif()

# ---- Add pytomlpp-test ----

ENABLE_TESTING()

add_test(NAME pytomlpp-test COMMAND pytomlpp-test)

# ---- code coverage ----

if (ENABLE_TEST_COVERAGE)
  target_compile_options(pytomlpp PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(pytomlpp PUBLIC -fprofile-arcs -ftest-coverage)
endif()
