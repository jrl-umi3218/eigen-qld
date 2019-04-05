#!/usr/bin/env python
# -*- coding: utf-8 -*-

import eigen
import eigen_qld

class QP1(object):
  def __init__(self):
    self.nrvar = 6
    self.nreq = 3
    self.nrineq = 2

    self.Q = eigen.MatrixXd.Identity(self.nrvar, self.nrvar)

    self.Aeq = eigen.MatrixXd([ [1., -1., 1., 0., 3., 1.],
                                [-1., 0., -3., -4., 5., 6.],
                                [2., 5., 3., 0., 1., 0.] ])
    self.Beq = eigen.VectorXd([1., 2., 3.])

    self.Aineq = eigen.MatrixXd([ [0., 1., 0., 1., 2., -1.],
                                  [-1., 0., 2., 1., 1., 0.] ])
    self.Bineq = eigen.VectorXd([-1., 2.5])

    self.XL = eigen.VectorXd([ -1000., -10000., 0., -1000., -1000.,-1000. ])
    self.XU = eigen.VectorXd([ 10000., 100., 1.5, 100., 100., 1000. ])

    self.C = eigen.VectorXd([ 1., 2., 3., 4., 5., 6. ])

    self.X = eigen.VectorXd([ 1.7975426, -0.3381487, 0.1633880, -4.9884023, 0.6054943, -3.1155623 ])

qp1 = QP1()
qld = eigen_qld.QLD(qp1.nrvar, qp1.nreq, qp1.nrineq)
qld.solve(qp1.Q, qp1.C, qp1.Aeq, qp1.Beq, qp1.Aineq, qp1.Bineq, qp1.XL, qp1.XU)
print("Result: {}".format(qld.result().transpose()))
print("   Ref: {}".format(qp1.X.transpose()))
