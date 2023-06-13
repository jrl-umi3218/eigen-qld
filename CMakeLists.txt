#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

cmake_minimum_required(VERSION 3.1.3)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME eigen-qld)
set(PROJECT_DESCRIPTION "QLD QP solver through Eigen3 library.")
set(PROJECT_URL "https://github.com/jrl-umi3218/eigen-qld")
set(PROJECT_VERSION 1.2.4)
set(PROJECT_USE_CMAKE_EXPORT TRUE)

include(cmake/base.cmake)
include(cmake/cython/cython.cmake)
include(cmake/msvc-specific.cmake)
include(cmake/version-script.cmake)

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
