project(contra-test-package)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP(TARGETS KEEP_RPATHS)

add_executable(contra-test-package main.cpp)
target_link_libraries(contra-test-package CONAN_PKG::contra)
