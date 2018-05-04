from conans import ConanFile, CMake
import os

class ContraTestPackage(ConanFile):
    name = "ContraTestPackage"
    license = "Apache License, Version 2.0"
    description = """A test for the conan recipe for contra"""
    settings = "os", "compiler", "build_type", "arch"

    requires = "contra/master@RWTH-VR/thirdparty"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')


    def test(self):
        os.chdir("bin")
        self.run(".%scontra-test-package" % os.sep)

