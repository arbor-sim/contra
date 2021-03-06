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


find_file(CONAN_COMMAND
  NAMES conan conan.exe
  PATHS ENV PATH ENV CONAN_DIR
)
if(CONAN_COMMAND)
  option(USE_CONAN "Use conan for dependency managment." ON)
else()
  option(USE_CONAN "Use conan for dependency managment." OFF)
  message(STATUS
    " NOTICE:\n"
    "      This project can use conan for dependency management.\n"
    "      Apparently, you do not have conan installed on your machine.\n"
    "      So you need to keep track of all dependencies.\n"
    "      In order to use conan, please install it:\n"
    "          pip install conan\n"
    "      More info: https://conan.io"
  )
endif()

if(USE_CONAN)
  include(conan)
  execute_process(COMMAND conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan False OUTPUT_QUIET)
  execute_process(COMMAND conan remote add rwth-vr https://api.bintray.com/conan/rwth-vr/conan False OUTPUT_QUIET)

  set(CONAN_OPTIONS)
  if (WITH_SHARED_MEMORY)
    list(APPEND CONAN_OPTIONS "with_shared_memory=True")
  endif (WITH_SHARED_MEMORY)
  if (WITH_ZEROMQ)
    list(APPEND CONAN_OPTIONS "with_zeromq=True")
  endif (WITH_ZEROMQ)

  conan_cmake_run(CONANFILE conanfile.py
                  BUILD missing
                  OPTIONS ${CONAN_OPTIONS})
endif()

#we reset CONAN_BUILD_INFO, so it is searched everytime, e.g. if we want to change from multi to a specific config
set (CONAN_BUILD_INFO "CONAN_BUILD_INFO-NOTFOUND")
find_file(CONAN_BUILD_INFO conanbuildinfo_multi.cmake
  NO_DEFAULT_PATH
  PATHS ${CMAKE_BINARY_DIR}
)
if(${CONAN_BUILD_INFO} STREQUAL  "CONAN_BUILD_INFO-NOTFOUND")
	find_file(CONAN_BUILD_INFO conanbuildinfo.cmake
	  NO_DEFAULT_PATH
	  PATHS ${CMAKE_BINARY_DIR}
	)
	if(NOT ${CONAN_BUILD_INFO} STREQUAL  "CONAN_BUILD_INFO-NOTFOUND")
		set( USE_NOT_MULTI_CONAN_BUILD_INFO TRUE)
	endif()
endif()


if(USE_CONAN AND NOT CONAN_BUILD_INFO)
  message(FATAL_ERROR
    " Error using conan:\n"
    "   You need to execute 'conan install' before running cmake!\n"
    "   Aborting.")
endif()


if(USE_CONAN AND CONAN_BUILD_INFO)
  include(${CONAN_BUILD_INFO})
  if(USE_NOT_MULTI_CONAN_BUILD_INFO)
	  conan_basic_setup(TARGETS NO_OUTPUT_DIRS KEEP_RPATHS)
  else()
    conan_basic_setup(TARGETS KEEP_RPATHS)
  endif()
endif()

macro(CONAN_FIND_PACKAGE target_name_var package)
  # parse arguments
  set(options)
  set(oneValueArgs)
  set(multiValueArgs)
  cmake_parse_arguments(CONAN_FIND_PACKAGE
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # set target variable to be used in target_link_libraries accordingly
  option(USE_CONAN_${package} "Use conan for dependency ${package}" ${USE_CONAN})

  if(USE_CONAN AND USE_CONAN_${package})
    set(${target_name_var} CONAN_PKG::${package})
  else()
    set(${target_name_var})
  endif()
endmacro()

