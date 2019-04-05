#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

project(qld_fortran Fortran)

set(QLD_FORT
  ../QLD/f/QLD.f
)

add_library(qld_fortran ${QLD_FORT})
set_target_properties(qld_fortran PROPERTIES IMPORT_SUFFIX ".lib")
install(
  TARGETS qld_fortran
  EXPORT eigen-qldTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
