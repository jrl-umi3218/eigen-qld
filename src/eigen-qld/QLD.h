/*
 * Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

// includes
// std
#include <cassert>

// Eigen
#include <Eigen/Core>

// eigen-qld
#include "eigen_qld_api.h"

namespace Eigen
{

extern "C" int ql_(const int * m,
                   const int * me,
                   const int * mmax,
                   const int * n,
                   const int * nmax,
                   const int * mnn,
                   const double * c,
                   const double * d,
                   const double * a,
                   const double * b,
                   const double * xl,
                   const double * xu,
                   double * x,
                   double * u,
                   const double * eps,
                   const int * mode,
                   const int * iout,
                   int * ifail,
                   const int * iprint,
                   double * war,
                   int * lwar,
                   int * iwar,
                   int * liwar);

class QLD
{
public:
  EIGEN_QLD_API QLD();
  EIGEN_QLD_API QLD(int nrvar, int nreq, int nrineq, bool verbose = false, int ldq=-1, int lda=-1);

  EIGEN_QLD_API void fdOut(int fd);
  EIGEN_QLD_API int fdOut() const;

  EIGEN_QLD_API void verbose(bool v);
  EIGEN_QLD_API bool verbose() const;

  EIGEN_QLD_API int fail() const;

  EIGEN_QLD_API void problem(int nrvar, int nreq, int nrineq, int ldq=-1, int lda=-1);

  EIGEN_QLD_API const VectorXd & result() const;

  /** Return the lagrange multipliers associated with results **/
  EIGEN_QLD_API const VectorXd & multipliers() const;

  template<typename MatObj,
           typename VecObj,
           typename MatEq,
           typename VecEq,
           typename MatIneq,
           typename VecIneq,
           typename VecVar>
  bool solve(const MatrixBase<MatObj> & Q,
             const MatrixBase<VecObj> & C,
             const MatrixBase<MatEq> & Aeq,
             const MatrixBase<VecEq> & Beq,
             const MatrixBase<MatIneq> & Aineq,
             const MatrixBase<VecIneq> & Bineq,
             const MatrixBase<VecVar> & XL,
             const MatrixBase<VecVar> & XU,
             bool isDecomp = false,
             double eps = 1e-12);

private:
  MatrixXd A_;
  VectorXd B_, X_;
  int fdOut_;
  int verbose_;
  int fail_;
  VectorXd U_;
  VectorXd WAR_;
  VectorXi IWAR_;
  EIGEN_QLD_API inline int fortran_ql(const int * m,
                                      const int * me,
                                      const int * mmax,
                                      const int * n,
                                      const int * nmax,
                                      const int * mnn,
                                      const double * c,
                                      const double * d,
                                      const double * a,
                                      const double * b,
                                      const double * xl,
                                      const double * xu,
                                      double * x,
                                      double * u,
                                      const double * eps,
                                      const int * mode,
                                      const int * iout,
                                      int * ifail,
                                      const int * iprint,
                                      double * war,
                                      int * lwar,
                                      int * iwar,
                                      int * liwar)
  {
    return ql_(m, me, mmax, n, nmax, mnn, c, d, a, b, xl, xu, x, u, eps, mode, iout, ifail, iprint, war, lwar, iwar,
               liwar);
  }
};

// inline

template<typename MatObj,
         typename VecObj,
         typename MatEq,
         typename VecEq,
         typename MatIneq,
         typename VecIneq,
         typename VecVar>
inline bool QLD::solve(const MatrixBase<MatObj> & Q,
                       const MatrixBase<VecObj> & C,
                       const MatrixBase<MatEq> & Aeq,
                       const MatrixBase<VecEq> & Beq,
                       const MatrixBase<MatIneq> & Aineq,
                       const MatrixBase<VecIneq> & Bineq,
                       const MatrixBase<VecVar> & XL,
                       const MatrixBase<VecVar> & XU,
                       bool isDecomp,
                       double eps)
{
  assert(Aeq.rows() == Beq.rows()); // check equality size
  assert(Aeq.cols() == X_.rows());
  assert(Aineq.rows() == Bineq.rows()); // check inequality size
  assert(Aineq.cols() == X_.rows());
  assert(Q.rows() == Q.cols()); // check Q is square
  assert(Q.cols() == X_.rows()); // check Q has the good number of variable
  assert(C.rows() == X_.rows()); // check C size
  assert(XU.rows() == X_.rows()); // check XL size
  assert(XL.rows() == X_.rows()); // check XU size

  int nreq = int(Beq.rows());
  int nrineq = int(Bineq.rows());
  int nrvar = int(X_.rows());

  int mode = isDecomp ? 0 : 1;

  int M = nreq + nrineq;
  int N = nrvar;

  int MMAX = int(A_.stride());
  // beware, don't work if base Q is not square
  int NMAX = int(Q.stride());

  int NMN = M + 2 * N;
  int LWAR = int(WAR_.rows());
  int LIWAR = int(IWAR_.rows());

  A_.block(0, 0, nreq, nrvar) = -Aeq;
  A_.block(nreq, 0, nrineq, nrvar) = -Aineq;

  B_.segment(0, nreq) = Beq;
  B_.segment(nreq, nrineq) = Bineq;

  fortran_ql(&M, &nreq, &MMAX, &N, &NMAX, &NMN, Q.derived().data(), C.derived().data(), A_.data(), B_.data(),
             XL.derived().data(), XU.derived().data(), X_.data(), U_.data(), &eps, &mode, &fdOut_, &fail_, &verbose_,
             WAR_.data(), &LWAR, IWAR_.data(), &LIWAR);

  return fail_ == 0;
}

} // namespace Eigen
