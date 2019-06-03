# eigen-qld

[![License](https://img.shields.io/badge/License-BSD%202--Clause-green.svg)](https://opensource.org/licenses/BSD-2-Clause)
[![Build Status](https://travis-ci.org/jrl-umi3218/eigen-qld.svg?branch=master)](https://travis-ci.org/jrl-umi3218/eigen-qld)
[![AppVeyor status](https://ci.appveyor.com/api/projects/status/wlds1vauvfvmfspj/branch/master?svg=true)](https://ci.appveyor.com/project/gergondet/eigen-qld/branch/master)
[ ![Download](https://api.bintray.com/packages/gergondet/multi-contact/eigen-qld%3Agergondet/images/download.svg) ](https://bintray.com/gergondet/multi-contact/eigen-qld%3Agergondet/_latestVersion)

eigen-qld provides an interface to use the [QLD QP solver](https://help.scilab.org/doc/5.5.2/en_US/qld.html) with the [Eigen3](https://eigen.tuxfamily.org) library.

Installing
------

## Ubuntu LTS (14.04, 16.04, 18.04): PPA

Use the [multi-contact-unstable](https://launchpad.net/~pierre-gergondet+ppa/+archive/ubuntu/multi-contact-unstable) ppa:
```bash
sudo add-apt-repository ppa:pierre-gergondet+ppa/multi-contact-unstable
sudo apt-get update
sudo apt-get install libeigen-qld-dev libeigen-qld-doc python-eigen-qld python3-eigen-qld
```


## Homebrew OS X install

Install from the command line using [Homebrew](brew.sh):

```bash
# install homebrew package manager
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
# install caskroom application manager
brew install caskroom/cask/brew-cask
# tap homebrew-science package repository
brew tap homebrew/science
# tap ahundt-robotics repository
brew tap ahundt/robotics
# install tasks and all its dependencies
brew install eigen-qld
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
 * `DISABLE_TESTS` Disable unit tests building (ON/OFF, default: OFF)
 * `USE_F2C` Build with fortran source code translated in C (slower runtime) (ON/OFF, default: OFF).
