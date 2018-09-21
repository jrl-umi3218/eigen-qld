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

SET(SETUP_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/")
IF(DEFINED CMAKE_BUILD_TYPE)
  FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
ELSE()
  FOREACH(CFG ${CMAKE_CONFIGURATION_TYPES})
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CFG}")
  ENDFOREACH()
ENDIF()
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/setup.in.py" "${CMAKE_CURRENT_BINARY_DIR}/setup.in.py")
FILE(GENERATE
      OUTPUT "${SETUP_LOCATION}/setup.py"
      INPUT "${CMAKE_CURRENT_BINARY_DIR}/setup.in.py")

# Build the bindings locally at build time for test purposes
add_custom_target(eigen-qld-python-bindings ALL
  COMMAND ${CMAKE_COMMAND} -E chdir "${SETUP_LOCATION}" python setup.py build_ext --inplace
  COMMENT "Generating local eigen-qld Python bindings"
  DEPENDS eigen_qld/c_eigen_qld.pxd eigen_qld/eigen_qld.pxd eigen_qld/eigen_qld.pyx
  SOURCES eigen_qld/c_eigen_qld.pxd eigen_qld/eigen_qld.pxd eigen_qld/eigen_qld.pyx
)

if(NOT ${DISABLE_TESTS})
  add_custom_target(test-eigen-qld-python-bindings
    COMMAND ${CMAKE_COMMAND} -E chdir "$<TARGET_FILE_DIR:RBDyn>" nosetests ${SETUP_LOCATION}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Test eigen-qld Python bindings"
    DEPENDS tests/test_eigen-qld_pickle.py
    SOURCES tests/test_eigen-qld_pickle.py
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
  COMMAND ${CMAKE_COMMAND} -E chdir "${SETUP_LOCATION}" pip install . ${PIP_EXTRA_OPTIONS}
  COMMENT "Install eigen-qld Python bindings"
)

install(CODE "execute_process(COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --config \${CMAKE_INSTALL_CONFIG_NAME} --target install-eigen-qld-python-bindings)")
