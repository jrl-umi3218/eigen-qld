# eigen-qld

[![License LGPL 3](https://img.shields.io/badge/license-LGPLv3-green.svg)](http://www.gnu.org/licenses/lgpl-3.0.txt)
[![Build Status](https://travis-ci.org/jrl-umi3218/eigen-qld.svg?branch=master)](https://travis-ci.org/jrl-umi3218/eigen-qld)
[![AppVeyor status](https://ci.appveyor.com/api/projects/status/wlds1vauvfvmfspj/branch/master?svg=true)](https://ci.appveyor.com/project/gergondet/eigen-qld/branch/master)

eigen-qld allow to use the QLD QP solver with the Eigen3 library.

## Installing

### Manual

#### Dependencies

To compile you need the following tools:

 * [Git]()
 * [CMake]() >= 2.8
 * [pkg-config]()
 * [doxygen]()
 * [g++]()
 * [gfortran]() (-DUSE_F2C=OFF)
 * [gcc]() (-DUSE_F2C=ON)
 * [Boost](http://www.boost.org/doc/libs/1_58_0/more/getting_started/unix-variants.html) >= 1.49
 * [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) >= 3.2

#### Building

```sh
git clone --recursive https://github.com/jrl-umi3218/eigen-qld
cd eigen-qld
mkdir _build
cd _build
cmake [options] ..
make && make intall
```

Where the main options are:

 * `-DCMAKE_BUIlD_TYPE=Release` Build in Release mode
 * `-DCMAKE_INSTALL_PREFIX=some/path/to/install` default is `/usr/local`
 * `-DUNIT_TESTS=ON` Build unit tests.
 * `-DUSE_F2C=ON` Build with fortran source code translated in C (slower runtime).

Pulling git subtree
------

To update cmake directory with their upstream git repository:

```
git fetch git://github.com/jrl-umi3218/jrl-cmakemodules.git master
git subtree pull --prefix cmake git://github.com/jrl-umi3218/jrl-cmakemodules.git master --squash
```

```
git fetch git://github.com/jrl-umi3218/jrl-travis.git master
git subtree pull --prefix .jrl-ci git://github.com/jrl-umi3218/jrl-travis.git master --squash
```
