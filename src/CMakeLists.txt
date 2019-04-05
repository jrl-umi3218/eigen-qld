#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

if(NOT (${USE_F2C} OR ${USE_FORTRAN_SUBDIRECTORY}))
  enable_language(Fortran)
elseif(${USE_F2C})
  enable_language(C)
endif()

set(SOURCES QLD.cpp)
set(HEADERS eigen-qld/eigen_qld_api.h eigen-qld/QLD.h)

set(QLD_FORT QLD/f/QLD.f)
set(QLD_C QLD/c/QLD.c)

if(${USE_F2C})
  list(APPEND SOURCES ${QLD_C})
elseif(NOT ${USE_FORTRAN_SUBDIRECTORY})
  list(APPEND SOURCES ${QLD_FORT})
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1 VERSION 1.0.0)

if(${USE_FORTRAN_SUBDIRECTORY})
  include(CMakeAddFortranSubdirectory)
  cmake_add_fortran_subdirectory(qld_fortran
    PROJECT qld_fortran  # project name in toplevel CMakeLists.txt in lapack
    ARCHIVE_DIR qld_fortran # .lib location relative to root binary tree
    RUNTIME_DIR qld_fortran # .dll location relative to root binary tree
    LIBRARIES qld_fortran # target libraries created
    LINK_LIBRARIES  # link interface libraries
    NO_EXTERNAL_INSTALL)
  target_link_libraries(${PROJECT_NAME} qld_fortran)
  if(WIN32)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qld_fortran/libqld_fortran.dll DESTINATION ${CMAKE_INSTALL_BINDIR})
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qld_fortran/libqld_fortran.lib DESTINATION ${CMAKE_INSTALL_LIBDIR})
  endif()
endif()

if(${USE_F2C})
  target_link_libraries(${PROJECT_NAME} libf2c.a)
endif()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION "${INCLUDE_INSTALL_DIR}"
)
install(FILES ${HEADERS} DESTINATION ${INCLUDE_INSTALL_DESTINATION})

