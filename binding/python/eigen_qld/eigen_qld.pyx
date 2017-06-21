# distutils: language = c++

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

cdef extern from "private_typedefs.h":
  pass

cimport c_eigen_qld

cimport eigen.eigen as eigen

cdef class QLD(object):
  def __cinit__(self, *args):
    if len(args) == 0:
      self.impl = c_eigen_qld.QLD()
    elif len(args) == 3:
      self.impl = c_eigen_qld.QLD(args[0], args[1], args[2])
    elif len(args) == 4:
      self.impl = c_eigen_qld.QLD(args[0], args[1], args[2], args[3])
    else:
      raise TypeError("Wrong arguments passed to QLD ctor")
  def problem(self, nrvar, nreq, nrineq):
    self.impl.problem(nrvar, nreq, nrineq)
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
