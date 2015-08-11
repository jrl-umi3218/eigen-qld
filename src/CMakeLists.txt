if(NOT (${USE_F2C} OR ${USE_FORTRAN_SUBDIRECTORY}))
  enable_language(Fortran)
elseif(${USE_F2C})
  enable_language(C)
endif()

set(SOURCES QLD.cpp)
set(HEADERS eigen_qld_api.h QLD.h)

set(QLD_FORT QLD/f/QLD.f)
set(QLD_C QLD/c/QLD.c)

if(${USE_F2C})
  list(APPEND SOURCES ${QLD_C})
elseif(NOT ${USE_FORTRAN_SUBDIRECTORY})
  list(APPEND SOURCES ${QLD_FORT})
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

if(${USE_FORTRAN_SUBDIRECTORY})
  include(CMakeAddFortranSubdirectory)
  cmake_add_fortran_subdirectory(win_fortran
    PROJECT win_fortran  # project name in toplevel CMakeLists.txt in lapack
    ARCHIVE_DIR win_fortran # .lib location relative to root binary tree
    RUNTIME_DIR win_fortran # .dll location relative to root binary tree
    LIBRARIES win_fortran # target libraries created
    LINK_LIBRARIES  # link interface libraries
    NO_EXTERNAL_INSTALL)
  target_link_libraries(${PROJECT_NAME} win_fortran)
endif()

if(${USE_F2C})
  target_link_libraries(${PROJECT_NAME} libf2c.a)
endif()

set(INSTALL_PATH include/${PROJECT_NAME})
install(FILES ${HEADERS} DESTINATION ${INSTALL_PATH})
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

