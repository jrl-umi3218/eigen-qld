#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport c_eigen_qld

cdef class QLD(object):
  cdef c_eigen_qld.QLD impl
