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

cmake_minimum_required(VERSION 3.6.0)

project(contra)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)

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


conan_or_find_package(boost_python REQUIRED)

option(WITH_TRANSPORT_ZEROMQ "build with ZEROMQ transport layer" OFF)
option(WITH_TRANSPORT_BOOST_SHARED_MEMORY 
       "build with boost shared memory transport layer" OFF)

if (WITH_TRANSPORT_ZEROMQ)
  conan_or_find_package(cppzmq REQUIRED)
  add_definitions(-DWITH_ZEROMQ_TRANSPORT=1)
  add_subdirectory(contra_zmq)
endif()

if (WITH_TRANSPORT_BOOST_SHARED_MEMORY)
  conan_or_find_package(boost_interprocess REQUIRED)
  add_definitions(-DWITH_BOOST_SHARED_MEMORY_TRANSPORT=1)
  add_subdirectory(contra_boost-shmem)
  add_subdirectory(helper_apps)
endif()

add_subdirectory(contra)
add_subdirectory(pycontra)
add_subdirectory(test_utilities)

get_property(ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
get_property(ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)

add_test_cpplint(NAME "contra-tests--cpplint"
    ${ALL_SOURCES} ${ALL_HEADERS}
  )

