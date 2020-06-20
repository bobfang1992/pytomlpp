cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

# ---- Project ----

project(
  pytomlpp
  LANGUAGES CXX
)
# Get version from VERSION file instead of hardcoding here
# This is so that we can maintain a single version across CMake and setup.py
file(STRINGS VERSION PYTOMLPP_VERSION)
set(PROJECT_VERSION ${PYTOMLPP_VERSION})
message("pytomlpp version: ${PYTOMLPP_VERSION}")

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ---- Dependencies ----
# For more info on CPM dependecies, see https://github.com/TheLartians/CPM.cmake

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage(
  NAME PackageProject.cmake
  GITHUB_REPOSITORY TheLartians/PackageProject.cmake
  VERSION 1.3
)

CPMAddPackage(
  NAME pybind11
  GITHUB_REPOSITORY pybind/pybind11
  VERSION 2.5.0
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

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files automatically.
# Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# ---- Create library ----

pybind11_add_module(pytomlpp ${headers} ${sources})
target_link_libraries(pytomlpp PRIVATE tomlplusplus pybind11)

set_target_properties(pytomlpp PROPERTIES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    PREFIX ""
)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(pytomlpp PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")

target_include_directories(pytomlpp
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed
# should match the project's regular header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  DEPENDENCIES ""
)
