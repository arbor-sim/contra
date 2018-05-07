#!/usr/bin/env python

import os
import sys

valid_stages = ["conan", "camke", "build", "test", "deliver"]
valid_os = ["Windows", "Linux", "OSX"]

valid_compilers = {
    "Windows": ["Visual Studio"],
    "Linux": ["gcc"],
    "OSX": ["apple-clang", "gcc"]
}

stage = ""
operating_system = ""
compiler = ""
compiler_version = ""
version = ""

def envvar(name):
    if operating_system == "Windows":
        return "%%%s%%" % name
    else:
        return "$%s" % name

def main(argv):
    if (len(argv) != 6):
        print("usage: .gitlab-ci.py [stage] [os] [compiler] [compiler_version] [version]")
        return -1

    stage = argv[1]
    operating_system = argv[2]
    compiler = argv[3]
    compiler_version = argv[4]
    version = argv[5]

    if not stage in valid_stages:
        print("Invalid stage, possible values: " + ', '.join(valid_stages))
        return -1

    if not operating_system in valid_os:
        print("Invalid operating system, possible values: " + ', '.join(valid_os))
        return -1

    if not compiler in valid_compilers[operating_system]:
        print("Invalid compiler, possible values: " + ', '.join(valid_compilers[operating_system]))
        return -1

    if stage == "conan":
        # if operating_system == "Windows":
        #     os.system("rmdir /s /q build")
        os.system("mkdir build")
        os.system("cd build")
        if operating_system == "Linux":
            os.system("export CC=gcc")
            os.system("export CXX=g++")
        os.system("conan remote update rwth-vr--bintray https://api.bintray.com/conan/rwth-vr/conan")
        os.system("conan user -p %s -r rwth-vr--bintray %s" % (envvar("CONAN_PASSWORD"), envvar("CONAN_LOGIN_USERNAME")))

        settings = []

        settings.append("-s compiler=\"%s\"" % compiler)
        settings.append("-s compiler.version=\"%s\"" % compiler_version)
        settings.append("-s arch=x86_64")
        settings.append("-s build_type=Release")

        if compiler == "Visual Studio":
            settings.append("-s compiler.runtime=MT")
        elif compiler == "gcc":
            settings.append("-s compiler.libcxx=\"libstdc++11\"")
        elif compiler == "apple-clang":
            settings.append("-s -s compiler.libcxx=libc++")            

        os.system("conan install --build=missing %s .." % ' '.join(settings))
    
if (__name__ == "__main__"):
    main(sys.argv)
