# eigen-qld

[![License](https://img.shields.io/badge/License-BSD%202--Clause-green.svg)](https://opensource.org/licenses/BSD-2-Clause)
[![Hosted By: Cloudsmith](https://img.shields.io/badge/OSS%20hosting%20by-cloudsmith-blue?logo=cloudsmith)](https://cloudsmith.com)
[![CI](https://github.com/jrl-umi3218/eigen-qld/workflows/CI%20of%20eigen-qld/badge.svg?branch=master)](https://github.com/jrl-umi3218/eigen-qld/actions?query=workflow%3A%22CI+of+eigen-qld%22)
[![Documentation](https://img.shields.io/badge/doxygen-online-brightgreen?logo=read-the-docs&style=flat)](http://jrl-umi3218.github.io/eigen-qld/doxygen/HEAD/index.html)

eigen-qld provides an interface to use the [QLD QP solver](https://help.scilab.org/doc/5.5.2/en_US/qld.html) with the [Eigen3](https://eigen.tuxfamily.org) library.

Installing
------

## Ubuntu LTS (16.04, 18.04, 20.04, 22.04, 24.04)

You must first setup our package mirror:

```
curl -1sLf \
  'https://dl.cloudsmith.io/public/mc-rtc/stable/setup.deb.sh' \
  | sudo -E bash
```

You can also choose the head mirror which will have the latest version of this package:

```
curl -1sLf \
  'https://dl.cloudsmith.io/public/mc-rtc/stable/setup.deb.sh' \
  | sudo -E bash
```

You can then install the package:

```bash
sudo apt install libeigen-qld-dev python-eigen-qld python3-eigen-qld
```

## Conan

Install the latest version using [conan](https://conan.io/)

```bash
conan remote add multi-contact https://api.bintray.com/conan/gergondet/multi-contact
# Install the latest release
conan install eigen-qld/latest@multi-contact/stable
# Or install the latest development version
# conan install eigen-qld/latest@multi-contact/dev
```

## Manually build from source

### Dependencies

To compile you need the following tools:

 * [Git]()
 * [CMake](cmake.org) >= 2.8
 * [pkg-config]()
 * [doxygen]()
 * [g++]()
 * [gfortran]() (`-DUSE_F2C=OFF`)
 * [gcc]() (`-DUSE_F2C=ON`)
 * [Boost](http://www.boost.org/doc/libs/1_58_0/more/getting_started/unix-variants.html) >= 1.49
 * [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) >= 3.2

For Python bindings:

 * [Cython](cython.readthedocs.io) >= 0.20
 * [Eigen3ToPython](https://github.com/jrl-umi3218/Eigen3ToPython) (to use the python binding)

### Building

```sh
git clone --recursive https://github.com/jrl-umi3218/eigen-qld
cd eigen-qld
mkdir _build
cd _build
cmake [options] ..
make && make intall
```

#### CMake options

By default, the build will use the `python` and `pip` command to install the bindings for the default system version (this behaviour can be used to build the bindings in a given virtualenv). The following options allow to control this behaviour:

 * `PYTHON_BINDING` Build the python binding (ON/OFF, default: ON)
 * `PYTHON_BINDING_FORCE_PYTHON2`: use `python2` and `pip2` instead of `python` and `pip`
 * `PYTHON_BINDING_FORCE_PYTHON3`: use `python3` and `pip3` instead of `python` and `pip`
 * `PYTHON_BINDING_BUILD_PYTHON2_AND_PYTHON2`: builds two sets of bindings one with `python2` and `pip2`, the other with `python3` and `pip3`
 * `BUILD_TESTING` Enable unit tests building (ON/OFF, default: ON)
 * `USE_F2C` Build with fortran source code translated in C (slower runtime) (ON/OFF, default: OFF).
