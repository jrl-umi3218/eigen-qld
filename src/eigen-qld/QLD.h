/*
 * Copyright 2012-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include "QLDDirect.h"

namespace Eigen
{

/** \brief A wrapper of the ql algorithm by Professor Schittkowski, with some convention
 * changes on the way the constraints are written.
 */
class QLD : public QLDDirect
{
public:
  /** Create a solver without allocating memory.
   * A call to QLD::problem will be necessary before calling QLD::solve
   */
  EIGEN_QLD_API QLD();

  /** Create a solver and allocate the memory necessary to solve a problem with the given dimensions.
   * See also QLD::problem
   */
  EIGEN_QLD_API QLD(int nrvar, int nreq, int nrineq, int ldq = -1, bool verbose = false);

  /** Allocate the memory necessary to solve a problem with the given dimensions.
   * \param nrvar Size of the variable vector \f$x\f$.
   * \param nreq Number of equality constraints, i.e. the row size of \f$A_{eq}\f$.
   * \param nrineq Number of inequality constraints, i.e. the row size of \f$A_{ineq}\f$.
   * \param ldq Leading dimension of the matrix \f$Q\f$ (see QLDDirect::problem). If
   * smaller than \p nrvar, \p nrvar will be used instead (which is the default case).
   *
   */
  EIGEN_QLD_API void problem(int nrvar, int nreq, int nrineq, int ldq = -1);

  /** Return the lagrange multipliers associated with results, with the multipliers
   * corresponding to equality constraints first, then to inequality constraints, then
   * lower bounds, then upper bounds.
   *
   * \warning These are the lagrange multipliers as computed by the underlying ql solver,
   * for which the constraints have the form \f$ A_{eq} + b_{eq} = 0\f$ and
   * \f$ A_{ineq} + b_{ineq} >= 0\f$.\n
   * For now, QLD::solve is changing automatically the constraints, but not the multipliers.
   * After a call to QLD::solveNoOverhead, the multipliers have the same meaning as for ql.
   */
  EIGEN_QLD_API const VectorXd & multipliers() const;

  /** Solve the problem
   * \f{align}{
   *   \underset{{x} \in \mathbb{R}^n}{\text{minimize}} & \ \frac{1}{2}{x^TQx} + {c^Tx} \nonumber \\
   *   \text{subject to} & \ A_{eq} x = b_{eq} \\
   *   & \ A_{ineq} x \leq b_{ineq} \\
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
   * \param Aeq The matrix \f$A_{eq}\f$.
   * \param beq The vector \f$b_{eq}\f$.
   * \param Aineq The matrix \f$A_{ineq}\f$.
   * \param bineq The vector \f$b_{ineq}\f$.
   * \param xl The vector \f$x_{l}\f$.
   * \param xu The vector \f$x_{u}\f$.
   * \param isDecomp specify if the Cholesky decomposition of \f$Q\f$ is used or not.
   * \param eps Desired final accuracy.
   *
   * This is a wrapper around QLDDirect::solver.
   */
  template<typename MatObj,
           typename VecObj,
           typename MatEq,
           typename VecEq,
           typename MatIneq,
           typename VecIneq,
           typename VecVar>
  bool solve(const MatrixBase<MatObj> & Q,
             const MatrixBase<VecObj> & c,
             const MatrixBase<MatEq> & Aeq,
             const MatrixBase<VecEq> & beq,
             const MatrixBase<MatIneq> & Aineq,
             const MatrixBase<VecIneq> & bineq,
             const MatrixBase<VecVar> & xl,
             const MatrixBase<VecVar> & xu,
             bool isDecomp = false,
             double eps = 1e-12);

private:
  MatrixXd A_;
  VectorXd B_;
};

template<typename MatObj,
         typename VecObj,
         typename MatEq,
         typename VecEq,
         typename MatIneq,
         typename VecIneq,
         typename VecVar>
inline bool QLD::solve(const MatrixBase<MatObj> & Q,
                       const MatrixBase<VecObj> & c,
                       const MatrixBase<MatEq> & Aeq,
                       const MatrixBase<VecEq> & beq,
                       const MatrixBase<MatIneq> & Aineq,
                       const MatrixBase<VecIneq> & bineq,
                       const MatrixBase<VecVar> & xl,
                       const MatrixBase<VecVar> & xu,
                       bool isDecomp,
                       double eps)
{
  assert(Aeq.rows() == beq.rows()); // check equality size
  assert(Aeq.cols() == A_.cols());
  assert(Aineq.rows() == bineq.rows()); // check inequality size
  assert(Aineq.cols() == A_.cols());

  int nreq = int(beq.rows());
  int nrineq = int(bineq.rows());

  A_.topRows(nreq) = -Aeq;
  A_.bottomRows(nrineq) = -Aineq;

  B_.head(nreq) = beq;
  B_.tail(nrineq) = bineq;

  return QLDDirect::solve(Q, c, A_, B_, xl, xu, nreq, isDecomp, eps);
}

} // namespace Eigen
