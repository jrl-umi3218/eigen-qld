#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

set(SETUP_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/")
if(DEFINED CMAKE_BUILD_TYPE)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
  foreach(CFG ${CMAKE_CONFIGURATION_TYPES})
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CFG}")
  endforeach()
endif()
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/setup.in.py" "${CMAKE_CURRENT_BINARY_DIR}/setup.in.py")
file(GENERATE
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

if(NOT ${DISABLE_TESTS})
  if(WIN32)
    set(PATH_SEP ";")
  else()
    set(PATH_SEP ":")
  endif()
  add_test(NAME PythonBindingsTest
    COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=$<TARGET_FILE_DIR:eigen-qld>${PATH_SEP}$ENV{LD_LIBRARY_PATH} nosetests -s ${SETUP_LOCATION}
  )
endif()

# Check whether pip install has a --system option
execute_process(COMMAND pip install --system
                RESULT_VARIABLE PIP_HAS_SYSTEM_INSTALL
                OUTPUT_QUIET ERROR_QUIET)
if(${PIP_HAS_SYSTEM_INSTALL} EQUAL 0)
  set(PIP_HAS_SYSTEM_INSTALL True)
else()
  set(PIP_HAS_SYSTEM_INSTALL False)
endif()

set(PIP_EXTRA_OPTIONS "")
if(${PYTHON_BINDING_USER_INSTALL})
  set(PIP_EXTRA_OPTIONS "--user")
endif()
if(DEFINED PIP_INSTALL_PREFIX)
  set(PIP_EXTRA_OPTIONS --prefix ${PIP_INSTALL_PREFIX})
  if(${PIP_HAS_SYSTEM_INSTALL})
    set(PIP_EXTRA_OPTIONS --system ${PIP_EXTRA_OPTIONS})
  endif()
endif()

# Install the bindings
add_custom_target(install-eigen-qld-python-bindings
  COMMAND ${CMAKE_COMMAND} -E chdir "${SETUP_LOCATION}" pip install . ${PIP_EXTRA_OPTIONS}
  COMMENT "Install eigen-qld Python bindings"
)

install(CODE "execute_process(COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --config \${CMAKE_INSTALL_CONFIG_NAME} --target install-eigen-qld-python-bindings)")
