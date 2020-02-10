/*
 * Copyright 2012-2020 CNRS-UM LIRMM, CNRS-AIST JRL
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

/** \brief A lightweight wrapper of the ql algorithm by Professor Schittkowski.
 * It handles the workspace memory to be passed to the solver along with the problem.
 */
class QLDDirect
{
public:
  /** Create a solver without allocating memory.
   * A call to QLDDirect::problem will be necessary before calling QLDDirect::solve
   */
  EIGEN_QLD_API QLDDirect();

  /** Create a solver and allocate the memory necessary to solve a problem with the given dimensions.
   * See also QLDDirect::problem
   */
  EIGEN_QLD_API QLDDirect(int nrvar, int nreq, int nrineq, int ldq = -1, int lda = -1, bool verbose = false);

  /** Specify a file number for output (Fortran unit specification).*/
  EIGEN_QLD_API void fdOut(int fd);
  /** Get the file number used for output*/
  EIGEN_QLD_API int fdOut() const;

  /** Specify if QLD must generate outputs or not.*/
  EIGEN_QLD_API void verbose(bool v);
  /** Check if QLD must generate outputs or not.*/
  EIGEN_QLD_API bool verbose() const;

  /** Return the fail code of the last call to QLDDirect::solve. 0 means succes.*/
  EIGEN_QLD_API int fail() const;

  /** Allocate the memory necessary to solve a problem with the given dimensions.
   * \param nrvar Size of the variable vector \f$x\f$.
   * \param nreq Number of equality constraints, i.e. the row size of \f$A_{eq}\f$.
   * \param nrineq Number of inequality constraints, i.e. the row size of \f$A_{ineq}\f$.
   * \param ldq Leading dimension of the matrix \f$Q\f$ (see below). If smaller than
   * \p nrvar, \p nrvar will be used instead (which is the default case).
   * \param lda Leading dimension of the matrix
   * \f$A = \begin{bmatrix} A_{eq} \\ A_{ineq} \end{bmatrix} \f$. If smaller
   * than \p nreq + \p nrineq, it will be set to this value (which is the default case).
   *
   * \note For a column-major matrix (which is the default case in Eigen and what QLD
   * is expecting), the matrix values are stored in a simple, 1-d array, one column
   * after the other. Often the column are place in this arry one right after the other
   * but this needs not be the case: there can be unused values in the array between
   * the values of two columns. The offset between the start of two consecutive columns
   * in the array is called the leading dimension. For a matrix \a M with leading dimension
   * \a ld, and data ordered in an array \a data, element M(i,j) is found at data[i+ld*j]. \n
   * As an example, consider the matrix
   * \f{align}{ \begin{bmatrix} 4 & 3 & 7 & 2 \\ 2 & 8 & 1 & 5 \\ 6 & 5 & 1 & 2 \end{bmatrix}
   * \f}
   * It can be stored with a leading dimension of 5 as \n
   * [4, 2, 6, x, x, 3, 8, 5, x, x, 7, 1, 1, x, x, 2, 5, 2] \n
   * where the x are unimportant value with respect to this matrix.\n
   * For classical Eigen matrices like \a MatrixXd, ld is equal to the row size, but if for
   * example we consider a matrix \a B that is the block of another one \a M, the
   * leading dimension of \a B will be the row dimension of \a M.
   */
  EIGEN_QLD_API void problem(int nrvar, int nreq, int nrineq, int ldq = -1, int lda = -1);

  /** Return the result from the latest call to QLDDirect::solve. */
  EIGEN_QLD_API const VectorXd & result() const;

  /** Return the lagrange multipliers associated with results, with the multipliers
   * corresponding to equality constraints first, then to inequality constraints, then
   * lower bounds, then upper bounds.
   */
  EIGEN_QLD_API const VectorXd & multipliers() const;

  /** Solve the problem
   * \f{align}{
   *   \underset{{x} \in \mathbb{R}^n}{\text{minimize}} & \ \frac{1}{2}{x^TQx} + {c^Tx} \nonumber \\
   *   \text{subject to} & \ A_{1:m_e} x + b_{1:m_e} = 0 \\
   *   & \ A_{m_e+1:\mbox{end}} x + b_{m_e+1:\mbox{end}} \geq 0 \\
   *   & \ x_l \leq x \leq x_u
   * \f}
   *
   * For a positive definite matrix \f$Q\f$, we have the Cholesky decomposition \f$Q = R^T R\f$
   * with \f$R\f$ upper triangular. If \p isDecomp is \a true, \f$R\f$ should be given instead of \f$Q\f$.
   * Only the upper triangular part of \f$R\f$ will be used.
   *
   * \param Q The matrix \f$Q\f$, or its Cholesky factor \f$R\f$ if \p isDecomp is \a true.
   * \f$Q\f$ should be symmetric and positive definite, \f$R\f$ should be upper triangular.
   * \param c The vector \f$c\f$.
   * \param A The matrix \f$A = \begin{bmatrix} A_{eq} \\ A_{ineq} \end{bmatrix} \f$.
   * \param b The vector \f$b = \begin{bmatrix} b_{eq} \\ b_{ineq} \end{bmatrix} \f$.
   * \param xl The vector \f$x_{l}\f$.
   * \param xu The vector \f$x_{u}\f$.
   * \param nreq The number \f$m_e\f$ of equality constraints (i.e. the row size of \f$A_{eq}\f$).
   * \param isDecomp specify if the Cholesky decomposition of \f$Q\f$ is used or not.
   * \param eps Desired final accuracy.
   */
  template<typename MatObj, typename VecObj, typename MatConstr, typename VecConstr, typename VecVar>
  bool solve(const MatrixBase<MatObj> & Q,
             const MatrixBase<VecObj> & c,
             const MatrixBase<MatConstr> & A,
             const MatrixBase<VecConstr> & b,
             const MatrixBase<VecVar> & xl,
             const MatrixBase<VecVar> & xu,
             int nreq,
             bool isDecomp = false,
             double eps = 1e-12);

private:
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

  VectorXd X_;
  int fdOut_;
  int verbose_;
  int fail_;
  VectorXd U_;
  VectorXd WAR_;
  VectorXi IWAR_;
};

template<typename MatObj, typename VecObj, typename MatConstr, typename VecConstr, typename VecVar>
inline bool QLDDirect::solve(const MatrixBase<MatObj> & Q,
                             const MatrixBase<VecObj> & c,
                             const MatrixBase<MatConstr> & A,
                             const MatrixBase<VecConstr> & b,
                             const MatrixBase<VecVar> & xl,
                             const MatrixBase<VecVar> & xu,
                             int nreq,
                             bool isDecomp,
                             double eps)
{
  assert(A.rows() == b.rows()); // check constraint size
  assert(A.cols() == X_.rows());
  assert(Q.rows() == Q.cols()); // check Q is square
  assert(Q.cols() == X_.rows()); // check Q has the good number of variable
  assert(c.rows() == X_.rows()); // check C size
  assert(xl.rows() == X_.rows()); // check XL size
  assert(xu.rows() == X_.rows()); // check XU size

  int mode = isDecomp ? 0 : 1;

  int M = int(A.rows());
  int N = int(X_.rows());

  int MMAX = std::max(int(A.stride()), 1);
  int NMAX = std::max(int(Q.stride()), 1);

  int NMN = M + 2 * N;
  int LWAR = int(WAR_.rows());
  int LIWAR = int(IWAR_.rows());

  assert(LWAR >= (3. * NMAX * NMAX) / 2. + 10. * NMAX + MMAX + M + 1.
         && "Please call QLD::problem with the correct dimensions.");
  assert(LIWAR >= N && "Please call QLD::problem with the correct dimensions.");

  fortran_ql(&M, &nreq, &MMAX, &N, &NMAX, &NMN, Q.derived().data(), c.derived().data(), A.derived().data(),
             b.derived().data(), xl.derived().data(), xu.derived().data(), X_.data(), U_.data(), &eps, &mode, &fdOut_,
             &fail_, &verbose_, WAR_.data(), &LWAR, IWAR_.data(), &LIWAR);

  return fail_ == 0;
}

} // namespace Eigen