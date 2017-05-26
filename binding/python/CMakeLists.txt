# Copyright 2012-2017 CNRS-UM LIRMM, CNRS-AIST JRL
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

macro(GET_EQLD_PROPERTY PROPERTY)
  get_target_property(EQLD_${PROPERTY} eigen-qld ${PROPERTY})
  if(NOT EQLD_${PROPERTY})
    set(EQLD_${PROPERTY} "")
  endif()
endmacro()
GET_EQLD_PROPERTY(COMPILE_FLAGS)
GET_EQLD_PROPERTY(INCLUDE_DIRECTORIES)
LIST(APPEND EQLD_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
GET_EQLD_PROPERTY(LINK_FLAGS)
GET_EQLD_PROPERTY(LINK_LIBRARIES)
GET_EQLD_PROPERTY(LOCATION)
configure_file(setup.in.py ${CMAKE_CURRENT_SOURCE_DIR}/setup.py)

# Build the bindings locally at build time for test purposes
add_custom_target(eigen-qld-python-bindings ALL
  COMMAND python setup.py build_ext --inplace
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating local eigen-qld Python bindings"
  DEPENDS eigen_qld/c_eigen_qld.pxd eigen_qld/eigen_qld.pxd eigen_qld/eigen_qld.pyx
)

if(NOT ${DISABLE_TESTS})
  add_custom_target(test-eigen-qld-python-bindings
    COMMAND nosetests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Test eigen-qld Python bindings"
    DEPENDS tests/test_eigen-qld_pickle.py
  )
  add_test(NAME PythonBindingsTest
    COMMAND nosetests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

set(PIP_EXTRA_OPTIONS "")
if(${PYTHON_BINDING_USER_INSTALL})
  set(PIP_EXTRA_OPTIONS "--user")
endif()
# Install the bindings
add_custom_target(install-eigen-qld-python-bindings
  COMMAND pip install . ${PIP_EXTRA_OPTIONS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Install eigen-qld Python bindings"
)
install(CODE "execute_process(COMMAND ${CMAKE_MAKE_PROGRAM} install-eigen-qld-python-bindings)")
