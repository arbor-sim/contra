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

find_package(PythonInterp)
if(NOT PYTHON_EXECUTABLE)
  message(SEND_ERROR
    " ERROR: Could not find any python interpreter.
        Having a python interpreter is a mandatory requirement for cpplint.
        CMake will not generate the project without it. ")
endif()

find_program(PY_TEST_COMMAND py.test pytest pytest.py py.test.py
  PATHS ENV PATH ENV PY_TEST_DIR
)
if(NOT PY_TEST_COMMAND)
  message(SEND_ERROR
    " ERROR: Could not find py.test.
        Having py.test is a mandatory requirement.
        CMake will not generate the project without it.
        Add its location to the environments variables PATH or PY_TEST_DIR.")
endif()

function(ADD_TEST_PY_TEST)
  set(options )
  set(oneValueArgs NAME PYTHONPATH)
  set(multiValueArgs )
  cmake_parse_arguments(ADD_TEST_PY_TEST
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  add_test(NAME "${ADD_TEST_PY_TEST_NAME}"
    COMMAND ${PYTHON_EXECUTABLE} -B ${PY_TEST_COMMAND} -p no:cacheprovider
    ${ADD_TEST_PY_TEST_UNPARSED_ARGUMENTS}
    
  )
  set_property(TEST ${ADD_TEST_PY_TEST_NAME} PROPERTY ENVIRONMENT
    "PYTHONPATH=${ADD_TEST_PY_TEST_PYTHONPATH}"
    )
endfunction()
