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

cmake_minimum_required(VERSION 3.6.0)

project(contra)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
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

if(UNIX AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(ConanHelpers)
include(CTest)
include(GenerateExportHeader)
include(rwthvr)

enable_testing()

conan_find_package(catch_target catch)
if(NOT catch_target)
  find_package(Catch2 REQUIRED)
  set(catch_target Catch2::Catch2)
endif()

conan_find_package(conduit_target conduit)
if(NOT conduit_target)
  find_package(conduit REQUIRED)
  set(conduit_target conduit)
  set_target_properties(${conduit_target} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CONDUIT_INCLUDE_DIRS})
  message(STATUS "CONDUIT_INCLUDE_DIRS ${CONDUIT_INCLUDE_DIRS}")
endif()

conan_find_package(cpplint_target cpplint)
if (NOT cpplint_target)
  find_package(cpplint QUIET)
  if(cpplint_FOUND)
    set(cpplint_target cpplint)
  endif()
endif()
if (NOT cpplint_target)
  message(WARNING "cpplint package not found: disabling cpplint tests")
else()
  include(cpplint)
endif()

conan_find_package(cppcheck_target cppcheck)
include(cppcheck)

find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

include(py.test)
include(python_module)

conan_find_package(boost_python_target boost_python)
if (NOT boost_python_target)
  find_package(Boost REQUIRED COMPONENTS python37)
  set(boost_python_target Boost::python37)
endif()

option(WITH_ZEROMQ "build with ZEROMQ transport layer" OFF)
option(WITH_SHARED_MEMORY
       "build with boost shared memory transport layer" OFF)

if (WITH_ZEROMQ)
  conan_find_package(cppzmq_target cppzmq)
  if (NOT cppzmq_target)
    find_package(cppzmq)
    set(cppzmq_target cppzmq)
  endif()
  add_definitions(-DWITH_ZEROMQ=1)
  add_subdirectory(contra_zmq)
endif()

if (WITH_SHARED_MEMORY)
  conan_find_package(boost_interprocess_target boost_interprocess)
  if (NOT boost_interprocess_target)
    find_package(Boost REQUIRED)
    set(boost_interprocess_target Boost::boost)
  endif()

  add_definitions(-DWITH_SHARED_MEMORY=1)
  add_subdirectory(contra_boost-shmem)
  add_subdirectory(helper_apps)
endif()

add_subdirectory(contra)
add_subdirectory(pycontra)
add_subdirectory(test_utilities)

get_property(ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
get_property(ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)

if (cpplint_target)
  add_test_cpplint(NAME "contra-tests--cpplint"
      ${ALL_SOURCES} ${ALL_HEADERS}
    )
endif()

install(
  FILES ${CMAKE_SOURCE_DIR}/cmake/contra-config.cmake
  DESTINATION lib/contra/cmake)
