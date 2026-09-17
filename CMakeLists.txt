#
# Copyright 2012-2026 CNRS-UM LIRMM, CNRS-AIST JRL
#

cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME eigen-qld)
set(PROJECT_DESCRIPTION "QLD QP solver through Eigen3 library.")
set(PROJECT_URL "https://github.com/jrl-umi3218/eigen-qld")
set(PROJECT_VERSION 1.2.7)
set(PROJECT_USE_CMAKE_EXPORT TRUE)

# Check if the submodule cmake have been initialized
set(JRL_CMAKE_MODULES "${CMAKE_CURRENT_LIST_DIR}/cmake")
if(EXISTS "${JRL_CMAKE_MODULES}/base.cmake")
  message(STATUS "JRL cmakemodules found in 'cmake/' git submodule")
else()
  find_package(jrl-cmakemodules QUIET CONFIG)
  if(jrl-cmakemodules_FOUND)
    get_property(
      JRL_CMAKE_MODULES
      TARGET jrl-cmakemodules::jrl-cmakemodules
      PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "JRL cmakemodules found on system at ${JRL_CMAKE_MODULES}")
  else()
    message(STATUS "JRL cmakemodules not found. Let's fetch it.")
    include(FetchContent)
    FetchContent_Declare(
      "jrl-cmakemodules"
      GIT_REPOSITORY "https://github.com/arntanguy/jrl-cmakemodules.git"
      GIT_TAG 98c3f2676abcd84775863f27a8eabcd864115b49)
    FetchContent_MakeAvailable("jrl-cmakemodules")
    FetchContent_GetProperties("jrl-cmakemodules" SOURCE_DIR JRL_CMAKE_MODULES)
  endif()
endif()
include(${JRL_CMAKE_MODULES}/base.cmake)
include(${JRL_CMAKE_MODULES}/msvc-specific.cmake)
include(${JRL_CMAKE_MODULES}/version-script.cmake)
if(${PYTHON_BINDING})
  include(${JRL_CMAKE_MODULES}/cython/cython.cmake)
endif()

# Disable -Werror on Unix for now.
set(CXX_DISABLE_WERROR True)
set(DOXYGEN_USE_MATHJAX "YES")

option(USE_F2C "Use f2c converted code (usually slower)." OFF)

if(WIN32)
  set(USE_FORTRAN_SUBDIRECTORY_DEFAULT ON)
else()
  set(USE_FORTRAN_SUBDIRECTORY_DEFAULT OFF)
endif()
option(USE_FORTRAN_SUBDIRECTORY
       "Use cmake to generate sub project for fortran parts"
       ${USE_FORTRAN_SUBDIRECTORY_DEFAULT})

project(eigen-qld C CXX)

add_project_dependency(Eigen3 REQUIRED)

# For MSVC, set local environment variable to enable finding the built dll of
# the main library when launching ctest with RUN_TESTS
if(MSVC)
  if(USE_FORTRAN_SUBDIRECTORY)
    set(CMAKE_MSVCIDE_RUN_PATH
        "\$(SolutionDir)/src/\$(Configuration);\$(SolutionDir)/src/qld_fortran")
  else()
    set(CMAKE_MSVCIDE_RUN_PATH "\$(SolutionDir)/src/\$(Configuration)")
  endif()
endif(MSVC)

add_subdirectory(src)

if(${BUILD_TESTING})
  add_subdirectory(tests)
endif()

if(${PYTHON_BINDING})
  add_subdirectory(binding/python)
endif()
