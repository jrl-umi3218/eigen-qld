/*
 * Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#ifdef WIN32
#  define EIGEN_QLD_DLLIMPORT __declspec(dllimport)
#  define EIGEN_QLD_DLLEXPORT __declspec(dllexport)
#else
#  define EIGEN_QLD_DLLIMPORT
#  define EIGEN_QLD_DLLEXPORT
#endif

#ifdef EIGEN_QLD_EXPORT
#  define EIGEN_QLD_API EIGEN_QLD_DLLEXPORT
#else
#  define EIGEN_QLD_API EIGEN_QLD_DLLIMPORT
#endif
