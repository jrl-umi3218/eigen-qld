# distutils: language = c++

#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

cdef extern from "private_typedefs.h":
  pass

cimport eigen_qld.c_eigen_qld as c_eigen_qld

cimport eigen.eigen as eigen

cdef class QLD(object):
  def __cinit__(self, *args):
    if len(args) == 0:
      self.impl = c_eigen_qld.QLD()
    elif len(args) == 3:
      self.impl = c_eigen_qld.QLD(args[0], args[1], args[2])
    elif len(args) == 4:
      self.impl = c_eigen_qld.QLD(args[0], args[1], args[2], -1, args[3])
    else:
      raise TypeError("Wrong arguments passed to QLD ctor")
  def problem(self, nrvar, nreq, nrineq):
    self.impl.problem(nrvar, nreq, nrineq, -1)
  def result(self):
    return eigen.VectorXdFromC(self.impl.result())
  def multipliers(self):
    return eigen.VectorXdFromC(self.impl.multipliers())
  def solve(self, eigen.MatrixXd Q, eigen.VectorXd C,
                  eigen.MatrixXd Aeq, eigen.VectorXd Beq,
                  eigen.MatrixXd Aineq, eigen.VectorXd Bineq,
                  eigen.VectorXd XL, eigen.VectorXd XU,
                  isDecomp=False, eps=1e-12):
    return self.impl.solve(Q.impl, C.impl, Aeq.impl, Beq.impl, Aineq.impl, Bineq.impl, XL.impl, XU.impl, isDecomp, eps)
