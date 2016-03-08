# Copyright 2015-2016 CNRS-UM LIRMM, CNRS-AIST JRL
#
# This file is part of eigen-qld.
#
# eigen-qld is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# eigen-qld is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with eigen-qld.  If not, see <http://www.gnu.org/licenses/>.

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
