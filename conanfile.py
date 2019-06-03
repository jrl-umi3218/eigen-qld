# -*- coding: utf-8 -*-
#
# Copyright 2012-2019 CNRS-UM LIRMM, CNRS-AIST JRL
#

from conans import python_requires
import conans.tools as tools
from conans.tools import SystemPackageTool, Version
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

    # build_requirements and system_requirements taken from https://github.com/conan-community/conan-lapack to fetch a Fortran compiler
    def build_requirements(self):
        if self.settings.os == "Windows":
            self.build_requires("mingw_installer/1.0@conan/stable")

    def system_requirements(self):
        installer = SystemPackageTool()
        if tools.os_info.is_linux:
            if tools.os_info.with_pacman or \
                tools.os_info.with_yum:
                installer.install("gcc-fortran")
            else:
                installer.install("gfortran")
                versionfloat = Version(self.settings.compiler.version.value)
                if self.settings.compiler == "gcc":
                    if versionfloat < "5.0":
                        installer.install("libgfortran-{}-dev".format(versionfloat))
                    else:
                        installer.install("libgfortran-{}-dev".format(int(versionfloat)))
        if tools.os_info.is_macos and Version(self.settings.compiler.version.value) > "7.3":
            try:
                installer.install("gcc", update=True, force=True)
            except Exception:
                self.output.warn("brew install gcc failed. Tying to fix it with 'brew link'")
                self.run("brew link --overwrite gcc")
