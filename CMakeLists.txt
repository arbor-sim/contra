#-------------------------------------------------------------------------------
# contra -- a lightweigth transport library for conduit data
#
# Copyright (c) 2018 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
#-------------------------------------------------------------------------------
#                                  License
#
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
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.8.2)

project(contra)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)

if(APPLE)
  set(CMAKE_MACOSX_RPATH FALSE)

# The following settings were copied from
# https://cmake.org/Wiki/CMake_RPATH_handling
# to avoid the rpath issue that appears on OS X El Capitan

  # use, i.e. don't skip the full RPATH for the build tree
  set(CMAKE_SKIP_BUILD_RPATH  FALSE)

  # when building, don't use the install RPATH already
  # (but later on when installing)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) # Changed to TRUE by A.O. 

  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

  # the RPATH to be used when installing, but only if it's not a system directory
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
     set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif("${isSystemDir}" STREQUAL "-1")
endif()

if (MSVC)
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif (MSVC)

include(ConanHelpers)
include(CTest)
include(GenerateExportHeader)
include(rwthvr)

enable_testing()

conan_or_find_package(catch REQUIRED)
conan_or_find_package(conduit REQUIRED)

conan_or_find_package(cpplint REQUIRED)
include(cpplint)

conan_or_find_package(cppcheck REQUIRED)
include(cppcheck)

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

include(py.test)
include(python_module)

conan_or_find_package(boost_interprocess REQUIRED)
conan_or_find_package(boost_python REQUIRED)

add_subdirectory(contra)
add_subdirectory(helper_apps)
add_subdirectory(pycontra)

get_property(ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
get_property(ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)

add_test_cppcheck(NAME "contra-tests--cppcheck"
    ${ALL_SOURCES}
  )
  
add_test_cpplint(NAME "contra-tests--cpplint"
    ${ALL_SOURCES} ${ALL_HEADERS}
  )

