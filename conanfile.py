# ------------------------------------------------------------------------------
# contra -- a lightweight transport library for conduit data
#
# Copyright (c) 2018 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
# ------------------------------------------------------------------------------
#                                  License
#
# The license of the software changes depending on if it is compiled with or
# without ZeroMQ support. See the LICENSE file for more details.
# ------------------------------------------------------------------------------
#                          Apache License, Version 2.0
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------
# Contra is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Contra is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Contra.  If not, see <https://www.gnu.org/licenses/>.
# ------------------------------------------------------------------------------

from conans import ConanFile, CMake


class contra(ConanFile):
    name = "contra"
    version = "18.07"
    license = "Apache License, Version 2.0 if with_transport_zeromq=False, LGPL-3.0 otherwise"
    description = """Contra, a lightweight transport library for conduit data"""
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*"
    url = "https://devhub.vr.rwth-aachen.de/VR-Group/contra"
    options = {"with_transport_boost_shared_memory": [
        True, False], "with_transport_zeromq": [True, False]}
    default_options = "with_transport_boost_shared_memory=True", "with_transport_zeromq=True"

    generators = "cmake"

    def requirements(self):
        self.requires("catch/1.12.0@RWTH-VR/thirdparty")
        self.requires("cpplint/e8ffd7c@RWTH-VR/thirdparty")
        self.requires("cppcheck/1.84@RWTH-VR/thirdparty")
        self.requires("conduit/0.3.1@RWTH-VR/thirdparty")
        self.requires("boost_python/1.66.0@bincrafters/testing")
        if (self.options.with_transport_boost_shared_memory):
            self.requires("boost_interprocess/1.66.0@bincrafters/testing")
        if (self.options.with_transport_zeromq):
            self.requires("cppzmq/4.2.2@bincrafters/stable")

    def configure(self):
        self.options["boost"].header_only = False
        self.options["boost"].shared = True
        self.options["boost_python"].python_version = 2.7
        if (self.settings.os == "Windows"):
            self.options["boost_python"].shared = False
        else:
            self.options["boost_python"].shared = True
        self.options["conduit"].shared = False

    def imports(self):
        self.copy("*.dll", dst="contra/tests/Debug", src="bin")
        self.copy("*.dll", dst="contra/tests/Release", src="bin")
        self.copy("*.dll", dst="contra/tests", src="bin")
        self.copy("*.dll", dst="pycontra/pycontra", src="bin")
        self.copy("*.so", dst="contra/tests", src="lib")
        self.copy("*.dylib", dst="contra/tests", src="lib")
        self.copy("*.dylib", dst="pycontra/pycontra", src="lib")

    def build(self):
        cmake = CMake(self)
        if (self.options.with_transport_boost_shared_memory) and (self.options.with_transport_zeromq):
            cmake.configure(
                defs='-DWITH_TRANSPORT_BOOST_SHARED_MEMORY=ON -DWITH_TRANSPORT_ZEROMQ=ON', source_folder='.')
        elif (self.options.with_transport_boost_shared_memory):
            cmake.configure(
                defs='-DWITH_TRANSPORT_BOOST_SHARED_MEMORY=ON', source_folder='.')
        elif (self.options.with_transport_zeromq):
            cmake.configure(defs='-DWITH_TRANSPORT_ZEROMQ=ON',
                            source_folder='.')
        else:
            cmake.configure(source_folder='.')
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include", src="contra/include")
        self.copy("*.hpp", dst="include", src="build/contra/include")
        if (self.options.with_transport_boost_shared_memory):
            self.copy("*.hpp", dst="include", src="contra_boost-shmem/include")
            self.copy("*.hpp", dst="include",
                      src="build/contra_boost-shmem/include")
        if (self.options.with_transport_zeromq):
            self.copy("*.hpp", dst="include", src="contra_zmq/include")
            self.copy("*.hpp", dst="include", src="build/contra_zmq/include")

        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so*", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False, symlinks=True)

    def package_info(self):
        if (self.options.with_transport_boost_shared_memory) and (self.options.with_transport_zeromq):
            self.cpp_info.libs = ["contra", "contra_boost-shmem", "contra_zmq"]
        elif not(self.options.with_transport_zeromq):
            self.cpp_info.libs = ["contra",  "contra_boost-shmem"]
        elif not(self.options.with_transport_boost_shared_memory):
            self.cpp_info.libs = ["contra",  "contra_zmq"]
        else:
            self.cpp_info.libs = ["contra"]
