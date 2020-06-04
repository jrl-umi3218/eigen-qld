#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

if(NOT (${USE_F2C} OR ${USE_FORTRAN_SUBDIRECTORY}))
  enable_language(Fortran)
elseif(${USE_F2C})
  enable_language(C)
endif()

set(SOURCES QLD.cpp QLDDirect.cpp)
set(HEADERS eigen-qld/eigen_qld_api.h eigen-qld/QLD.h eigen-qld/QLDDirect.h)

set(QLD_FORT QLD/f/QLD.f)
set(QLD_C QLD/c/QLD.c)

if(${USE_F2C})
  list(APPEND SOURCES ${QLD_C})
elseif(NOT ${USE_FORTRAN_SUBDIRECTORY})
  list(APPEND SOURCES ${QLD_FORT})
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(eigen-qld PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION})
set_target_properties(eigen-qld PROPERTIES COMPILE_FLAGS "-DEIGEN_QLD_EXPORT")
if(TARGET Eigen3::Eigen)
  target_link_libraries(eigen-qld PUBLIC Eigen3::Eigen)
else()
  target_include_directories(eigen-qld SYSTEM PUBLIC "${EIGEN3_INCLUDE_DIR}")
endif()

if(${USE_FORTRAN_SUBDIRECTORY})
  include(CMakeAddFortranSubdirectory)
  cmake_add_fortran_subdirectory(qld_fortran
    PROJECT qld_fortran  # project name in toplevel CMakeLists.txt in lapack
    ARCHIVE_DIR qld_fortran # .lib location relative to root binary tree
    RUNTIME_DIR qld_fortran # .dll location relative to root binary tree
    LIBRARIES qld_fortran # target libraries created
    LINK_LIBRARIES  # link interface libraries
    NO_EXTERNAL_INSTALL)
  set(qld_fortran_SHARED_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/qld_fortran/libqld_fortran${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(qld_fortran_STATIC_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/qld_fortran/libqld_fortran${CMAKE_STATIC_LIBRARY_SUFFIX})
  # Required here because qld_fortran is external to this project and install does not run
  if(WIN32)
    install(FILES ${qld_fortran_SHARED_LIBRARY} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${qld_fortran_STATIC_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    set(PACKAGE_EXTRA_MACROS "
get_filename_component(PACKAGE_PREFIX_DIR \"\${CMAKE_CURRENT_LIST_DIR}/../../../\" ABSOLUTE)
if(NOT TARGET qld_fortran)
  add_library(qld_fortran STATIC IMPORTED)
  set_target_properties(qld_fortran PROPERTIES
    IMPORTED_LOCATION \"\${PACKAGE_PREFIX_DIR}/lib/libqld_fortran${CMAKE_STATIC_LIBRARY_SUFFIX}\"
  )
endif()
"
        PARENT_SCOPE)
  endif()
  target_link_libraries(${PROJECT_NAME} PUBLIC qld_fortran)
endif()

if(${USE_F2C})
  target_link_libraries(${PROJECT_NAME} PUBLIC libf2c.a)
endif()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES ${HEADERS} DESTINATION ${INCLUDE_INSTALL_DESTINATION})

