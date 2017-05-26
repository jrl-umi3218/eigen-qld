// Copyright 2015-2016 CNRS-UM LIRMM, CNRS-AIST JRL
//
// This file is part of eigen-qld.
//
// eigen-qld is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// eigen-qld is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with eigen-qld.  If not, see <http://www.gnu.org/licenses/>.

// includes
// std
#include <iostream>

// boost
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Eigen_QLD
#include <boost/test/unit_test.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/floating_point_comparison.hpp>

// Eigen
#include <Eigen/Dense>

// EigenQP
#include <eigen-qld/QLD.h>


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


void ineqWithXBounds(Eigen::MatrixXd& Aineq, Eigen::VectorXd& Bineq,
	const Eigen::VectorXd& XL, const Eigen::VectorXd& XU)
{
	double inf = std::numeric_limits<double>::infinity();

	std::vector<std::pair<int, double> > lbounds, ubounds;

	for(int i = 0; i < XL.rows(); ++i)
	{
		if(XL[i] != -inf)
			lbounds.emplace_back(i, XL[i]);
		if(XU[i] != inf)
			ubounds.emplace_back(i, XU[i]);
	}

	long int nrconstr = Bineq.rows() + static_cast<long int>(lbounds.size()) +
		static_cast<long int>(ubounds.size());

	Eigen::MatrixXd A(Eigen::MatrixXd::Zero(nrconstr, Aineq.cols()));
	Eigen::VectorXd B(Eigen::VectorXd::Zero(nrconstr));

	A.block(0, 0, Aineq.rows(), Aineq.cols()) = Aineq;
	B.segment(0, Bineq.rows()) = Bineq;

	int start = static_cast<int>(Aineq.rows());

	for(int i = 0; i < static_cast<int>(lbounds.size()); ++i)
	{
		const auto& b = lbounds[i];
		A(start, b.first) = -1.;
		B(start) = -b.second;
		++start;
	}

	for(int i = 0; i < static_cast<int>(ubounds.size()); ++i)
	{
		const auto& b = ubounds[i];
		A(start, b.first) = 1.;
		B(start) = b.second;
		++start;
	}

	Aineq = A;
	Bineq = B;
}



BOOST_AUTO_TEST_CASE(QLD)
{
	QP1 qp1;

	Eigen::QLD qld(qp1.nrvar, qp1.nreq, qp1.nrineq);

	qld.solve(qp1.Q, qp1.C,
		qp1.Aeq, qp1.Beq,
		qp1.Aineq, qp1.Bineq,
		qp1.XL, qp1.XU);

	BOOST_CHECK_SMALL((qld.result() - qp1.X).norm(), 1e-6);
}



BOOST_AUTO_TEST_CASE(QLDSize)
{
	QP1 qp1;

	Eigen::QLD qld(qp1.nrvar, qp1.nreq + 10, qp1.nrineq + 22);

	qld.solve(qp1.Q, qp1.C,
		qp1.Aeq, qp1.Beq,
		qp1.Aineq, qp1.Bineq,
		qp1.XL, qp1.XU);

	BOOST_CHECK_SMALL((qld.result() - qp1.X).norm(), 1e-6);
}



BOOST_AUTO_TEST_CASE(IneqWithXBounds)
{
	QP1 qp1;

	ineqWithXBounds(qp1.Aineq, qp1.Bineq, qp1.XL, qp1.XU);

	double inf = std::numeric_limits<double>::infinity();
	for(int i = 0; i < qp1.nrvar; ++i)
	{
		qp1.XL[i] = -inf;
		qp1.XU[i] = inf;
	}

	int nrineq = static_cast<int>(qp1.Aineq.rows());
	Eigen::QLD qld(qp1.nrvar, qp1.nreq, nrineq);

	qld.solve(qp1.Q, qp1.C,
		qp1.Aeq, qp1.Beq,
		qp1.Aineq, qp1.Bineq,
		qp1.XL, qp1.XU);

	BOOST_CHECK_SMALL((qld.result() - qp1.X).norm(), 1e-6);
}
