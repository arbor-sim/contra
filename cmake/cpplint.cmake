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


find_package(PythonInterp 2.7 REQUIRED)
if(NOT PYTHON_EXECUTABLE)
  message(SEND_ERROR
    " ERROR: Could not find any python interpreter.
        Having a python interpreter is a mandatory requirement for cpplint.
        CMake will not generate the project without it. ")
endif()
if(NOT (PYTHON_VERSION_MAJOR EQUAL 2 AND PYTHON_VERSION_MINOR EQUAL 7))
  message(SEND_ERROR
    " ERROR: Did not find python 2.7.
        Having python 2.7 is a mandatory requirement for cpplint.
        CMake will not generate the project without it. ")  
endif()

set(CONAN_CPPLINT_DIR)
if(TARGET CONAN_PKG::cpplint)
  get_property(CONAN_CPPLINT_DIR
    TARGET CONAN_PKG::cpplint
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    )
else()
  message("No cpplint found through conan, trying to find an installed one on the system")
endif()
set(CPPLINT_COMMAND NOTFOUND)
#this find_file also uses the direct CONAN_INCLUDE_DIRS to avoid generator expressions, furthermore the release one for multi config
find_file(CPPLINT_COMMAND cpplint.py
	PATHS ${CONAN_CPPLINT_DIR} ${CONAN_INCLUDE_DIRS_CPPLINT} ${CONAN_INCLUDE_DIRS_CPPLINT_RELEASE}
	$ENV{PATH} $ENV{CPPLINT_DIR}
  )
if(NOT CPPLINT_COMMAND)
  message(SEND_ERROR
    " ERROR: Could not find cpplint.py.
        Having cpplint.py is a mandatory requirement.
        CMake will not generate the project without it.
        Add its location to the environments variables PATH or CPPLINT_DIR.")
else()
  message(STATUS "Using cpplint from: ${CPPLINT_COMMAND}")
endif()

function(ADD_TEST_CPPLINT)
  set(options )
  set(oneValueArgs NAME)
  set(multiValueArgs )
  cmake_parse_arguments(ADD_TEST_CPPLINT
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  
  set(CPPLINT_ARGS "--quiet")
  if(MSVC)
    set(CPPLINT_OUTPUT "--output=vs7")
  else()
    set(CPPLINT_OUTPUT "")
  endif()

  add_test(NAME "${ADD_TEST_CPPLINT_NAME}"
    COMMAND ${PYTHON_EXECUTABLE} ${CPPLINT_COMMAND} ${CPPLINT_OUTPUT} ${CPPLINT_ARGS}
    ${ADD_TEST_CPPLINT_UNPARSED_ARGUMENTS}
    )
  set_tests_properties(${ADD_TEST_CPPLINT_NAME} PROPERTIES TIMEOUT 20.0)
endfunction()
