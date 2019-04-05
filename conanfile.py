# -*- coding: utf-8 -*-
#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

from conans import python_requires
import os

base = python_requires("RBDyn/1.1.0@gergondet/stable")


class EigenQLDConan(base.RBDynConan):
    name = "eigen-qld"
    version = "1.0.0"
    description = "QLD QP solver through Eigen3 library"
    topics = ("robotics", "optimization", "qp", "python")
    url = "https://github.com/jrl-umi3218/eigen-qld"
    homepage = "https://github.com/jrl-umi3218/eigen-qld"
    author = "Pierre Gergondet <pierre.gergondet@gmail.com>"
    license = "BSD-2-Clause"
    exports = ["LICENSE"]
    exports_sources = ["CMakeLists.txt", "conan/CMakeLists.txt", "binding/*", "cmake/*", "doc/*", "src/*"]
    generators = "cmake"
    settings = "os", "arch", "compiler", "build_type"
    options = { "python_version": ["2.7", "3.3", "3.4", "3.5", "3.6", "3.7"] }
    default_options = { "python_version": base.get_python_version() }

    requires = (
        "Eigen3ToPython/1.0.0@gergondet/stable"
    )
