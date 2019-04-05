#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

cimport eigen.c_eigen as c_eigen
from libcpp cimport bool

cdef extern from "<eigen-qld/QLD.h>" namespace "Eigen":
  cdef cppclass QLD:
    QLD()
    QLD(int, int, int)
    QLD(int, int, int, bool)

    void problem(int, int, int)

    const c_eigen.VectorXd& result()

    const c_eigen.VectorXd& multipliers()

    bool solve(const c_eigen.MatrixXd&, const c_eigen.VectorXd&,
        const c_eigen.MatrixXd&, const c_eigen.VectorXd&,
        const c_eigen.MatrixXd&, const c_eigen.VectorXd&,
        const c_eigen.VectorXd&, const c_eigen.VectorXd&)
    bool solve(const c_eigen.MatrixXd&, const c_eigen.VectorXd&,
        const c_eigen.MatrixXd&, const c_eigen.VectorXd&,
        const c_eigen.MatrixXd&, const c_eigen.VectorXd&,
        const c_eigen.VectorXd&, const c_eigen.VectorXd&,
        bool isDecomp, double eps)
