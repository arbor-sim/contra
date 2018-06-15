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

#download/build the missing dependencies
include(conan)
execute_process(COMMAND conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan False OUTPUT_QUIET)
execute_process(COMMAND conan remote add rwth-vr https://api.bintray.com/conan/rwth-vr/conan False OUTPUT_QUIET)
conan_cmake_run(CONANFILE conanfile.py
                BUILD missing)

find_file(CONAN_COMMAND
  NAMES conan conan.exe
  PATHS ENV PATH ENV CONAN_DIR
)
if(CONAN_COMMAND)
  option(USE_CONAN "Use conan for dependency managment." ON)
else()
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
	conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
  else()
    conan_basic_setup(TARGETS)
  endif()
endif()

macro(CONAN_OR_FIND_PACKAGE package)
  # parse arguments
  set(options)
  set(oneValueArgs CONAN_NAME)
  set(multiValueArgs)
  cmake_parse_arguments(CONAN_OR_FIND_PACKAGE
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  string(REPLACE ";" " " ADDITIONAL_PARAMS "${CONAN_OR_FIND_PACKAGE_UNPARSED_ARGUMENTS}")

  # set default conan target name if CONAN_NAME was not specified
  if(CONAN_OR_FIND_PACKAGE_CONAN_NAME)
    set(CONAN_PACKAGE_NAME CONAN_PKG::${CONAN_OR_FIND_PACKAGE_CONAN_NAME})
  else()
    set(CONAN_PACKAGE_NAME CONAN_PKG::${package})
  endif()

  # set target variable to be used in target_link_libraries accordingly
  option(USE_CONAN_${package} "Use conan for dependency ${package}" ${USE_CONAN})
  string(REPLACE "." "_"
    package_underscored
    ${package}
    )
  if(USE_CONAN AND USE_CONAN_${package})
    set(CONAN_OR_CMAKE_${package_underscored} ${CONAN_PACKAGE_NAME})
  else()
    find_package(${package} ${ADDITIONAL_PARAMS})
    set(CONAN_OR_CMAKE_${package_underscored} ${package})
  endif()
endmacro()

