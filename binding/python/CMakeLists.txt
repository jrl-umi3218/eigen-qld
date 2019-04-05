#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

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

add_dependencies(eigen-qld-python-bindings eigen-qld)

add_test(NAME PythonBindingsTest
  COMMAND ${CMAKE_COMMAND} -E chdir "$<TARGET_FILE_DIR:eigen-qld>" nosetests ${SETUP_LOCATION}
)

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
