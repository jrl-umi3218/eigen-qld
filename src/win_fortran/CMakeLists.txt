project(win_fortran Fortran)

set(QLD_FORT 
  ../QLD/f/QLD.f
)

add_library(win_fortran ${QLD_FORT})
install(TARGETS win_fortran
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib")
