#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

set(BOOST_COMPONENTS unit_test_framework)
search_for_boost()

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories(${Boost_INCLUDE_DIRS})

add_executable(QPTest QPTest.cpp)
target_link_libraries(QPTest ${Boost_LIBRARIES} ${PROJECT_NAME})
if(${USE_FORTRAN_SUBDIRECTORY})
  target_link_libraries(QPTest qld_fortran)
endif()
add_test(QPTestUnit QPTest)
# Adding a project configuration file (for MSVC only)
GENERATE_MSVC_DOT_USER_FILE(QPTest "$(SolutionDir)/src/qld_fortran")
