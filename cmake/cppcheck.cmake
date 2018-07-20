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

include(get_target_include_directories)

set(CPPCHECK_COMMAND NOTFOUND)
find_program(CPPCHECK_COMMAND NAMES cppcheck cppcheck.exe 
    PATHS ${CONAN_BIN_DIRS_CPPCHECK} ${CONAN_BIN_DIRS_CPPCHECK_RELEASE} 
    $ENV{PATH} $ENV{CPPCHECK_DIR})

if(NOT CPPCHECK_COMMAND)
  message(SEND_ERROR
    " ERROR: Could not find cppcheck.
    Having cppcheck is a mandatory requirement.
    CMake will not generate the project without it.
    Add its location to the environments variables PATH or CPPCHECK_DIR.
	However, this should normally be supplied by conan!")
else()
  message(STATUS "Use cppcheck from: ${CPPCHECK_COMMAND}")
endif()

set(CPPCHECK_ARGUMENTS --enable=warning,performance,portability,missingInclude,style --suppress=missingIncludeSystem --error-exitcode=1)
if(MSVC)
  list(APPEND CPPCHECK_ARGUMENTS --template=vs)
elseif(CLANG)
  list(APPEND CPPCHECK_ARGUMENTS --template=clang)
elseif(GCC)
  list(APPEND CPPCHECK_ARGUMENTS --template=gcc)
endif()

function(ADD_TEST_CPPCHECK)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs)
  cmake_parse_arguments(ARGS
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    get_target_property(SOURCES ${ARGS_TARGET} SOURCES)
    get_target_include_directories(INCLUDE_DIRECTORIES TARGET ${ARGS_TARGET})
    message("SOURCES ${SOURCES}")

    set(INCLUDE_DIRECTORIES_PARAMETERS "")
    foreach(VAL ${INCLUDE_DIRECTORIES})
      set(INCLUDE_DIRECTORIES_PARAMETERS ${INCLUDE_DIRECTORIES_PARAMETERS};-I;${VAL})
    endforeach(VAL ${INCLUDE_DIRECTORIES})
    message("INCLUDE_DIRECTORIES_PARAMETERS ${INCLUDE_DIRECTORIES_PARAMETERS}")

  add_test(NAME "${ARGS_TARGET}--cppcheck"
    COMMAND "${CPPCHECK_COMMAND}" ${CPPCHECK_ARGUMENTS} ${SOURCES} ${INCLUDE_DIRECTORIES_PARAMETERS})
  set_tests_properties(${ARGS_NAME} PROPERTIES TIMEOUT 20.0)
endfunction()
