#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(QPTest QPTest.cpp)
target_link_libraries(QPTest PRIVATE Boost::unit_test_framework ${PROJECT_NAME})
if(${USE_FORTRAN_SUBDIRECTORY})
  target_link_libraries(QPTest qld_fortran)
endif()
add_test(QPTestUnit QPTest)
# Adding a project configuration file (for MSVC only)
generate_msvc_dot_user_file(QPTest "$(SolutionDir)/src/qld_fortran")
