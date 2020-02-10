/*
 * Copyright 2012-2020 CNRS-UM LIRMM, CNRS-AIST JRL
 */

// associated header
#include "eigen-qld/QLD.h"

namespace Eigen
{

QLD::QLD() : QLDDirect(), A_(), B_() {}

QLD::QLD(int nrvar, int nreq, int nrineq, int ldq, bool verbose)
: QLDDirect(nrvar, nreq, nrineq, ldq, -1, verbose), A_(), B_()
{
  problem(nrvar, nreq, nrineq, ldq);
}

void QLD::problem(int nrvar, int nreq, int nrineq, int ldq)
{
  QLDDirect::problem(nrvar, nreq, nrineq, ldq);

  int nrconstr = nreq + nrineq;
  A_.resize(nrconstr, nrvar);
  B_.resize(nrconstr);
}

const VectorXd & QLD::multipliers() const
{
  return QLDDirect::multipliers();
}

} // namespace Eigen
