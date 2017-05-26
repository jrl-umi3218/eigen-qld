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

#pragma once

#ifdef WIN32
    #define EIGEN_QLD_DLLIMPORT __declspec(dllimport)
    #define EIGEN_QLD_DLLEXPORT __declspec(dllexport)
#else
    #define EIGEN_QLD_DLLIMPORT
    #define EIGEN_QLD_DLLEXPORT
#endif

#ifdef EIGEN_QLD_EXPORT
    #define EIGEN_QLD_API EIGEN_QLD_DLLEXPORT
#else
    #define EIGEN_QLD_API EIGEN_QLD_DLLIMPORT
#endif
