/*
 * Copyright 2012-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

// associated header
#include "eigen-qld/QLD.h"

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

QLDDirect::QLDDirect() : fdOut_(0), verbose_(false), fail_(0), U_(), WAR_(), IWAR_() {}

QLDDirect::QLDDirect(int nrvar, int nreq, int nrineq, int ldq, int lda, bool verbose)
: fdOut_(0), verbose_(verbose ? 1 : 0), fail_(0), U_(), WAR_(), IWAR_()
{
  problem(nrvar, nreq, nrineq, ldq, lda);
}

void QLDDirect::fdOut(int fd)
{
  fdOut_ = fd;
}

int QLDDirect::fdOut() const
{
  return fdOut_;
}

void QLDDirect::verbose(bool v)
{
  verbose_ = v ? 1 : 0;
}

bool QLDDirect::verbose() const
{
  return verbose_;
}

int QLDDirect::fail() const
{
  return fail_;
}

void QLDDirect::problem(int nrvar, int nreq, int nrineq, int ldq, int lda)
{
  int nrconstr = nreq + nrineq;

  if(ldq < nrvar) ldq = nrvar;
  if(lda < nrconstr) lda = nrconstr;

  int MMAX = lda == 0 ? 1 : lda;
  int NMAX = ldq == 0 ? 1 : ldq;

  X_.resize(nrvar);

  U_.resize(nrconstr + 2 * nrvar);
  int LWAR = static_cast<int>(std::ceil((3. * NMAX * NMAX) / 2. + 10. * NMAX + MMAX + nrconstr + 1.));
  WAR_.resize(LWAR);
  IWAR_.resize(nrvar);
}

const VectorXd & QLDDirect::result() const
{
  return X_;
}

const VectorXd & QLDDirect::multipliers() const
{
  return U_;
}

int QLDDirect::fortran_ql(const int * m,
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

} // namespace Eigen
