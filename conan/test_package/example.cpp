#include <eigen-qld/QLD.h>

#include <iostream>

struct QP1
{
  QP1()
  {
    nrvar = 6;
    nreq = 3;
    nrineq = 2;

    Q.resize(nrvar, nrvar);
    Aeq.resize(nreq, nrvar);
    Aineq.resize(nrineq, nrvar);

    C.resize(nrvar);
    Beq.resize(nreq);
    Bineq.resize(nrineq);
    XL.resize(nrvar);
    XU.resize(nrvar);
    X.resize(nrvar);


    Aeq << 1., -1., 1., 0., 3., 1.,
           -1., 0., -3., -4., 5., 6.,
           2., 5., 3., 0., 1., 0.;
    Beq << 1., 2., 3.;

    Aineq << 0., 1., 0., 1., 2., -1.,
             -1., 0., 2., 1., 1., 0.;
    Bineq << -1., 2.5;

    //with  x between ci and cs:
    XL << -1000., -10000., 0., -1000., -1000.,-1000.;
    XU << 10000., 100., 1.5, 100., 100., 1000.;

    //and minimize 0.5*x'*Q*x + p'*x with
    C << 1., 2., 3., 4., 5., 6.;
    Q.setIdentity();

    X << 1.7975426, -0.3381487, 0.1633880, -4.9884023, 0.6054943, -3.1155623;
  }

  int nrvar, nreq, nrineq;
  Eigen::MatrixXd Q, Aeq, Aineq;
  Eigen::VectorXd C, Beq, Bineq, XL, XU, X;
};

int main()
{
  QP1 qp1;
  Eigen::QLD qld(qp1.nrvar, qp1.nreq, qp1.nrineq);
  qld.solve(qp1.Q, qp1.C, qp1.Aeq, qp1.Beq, qp1.Aineq, qp1.Bineq, qp1.XL, qp1.XU);
  std::cout << "Result: " << qld.result().transpose() << "\n";
  std::cout << "   Ref: " << qp1.X.transpose() << "\n";
  return 0;
}
