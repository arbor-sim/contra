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

file(GLOB CONTRA_TEST_SOURCES *.cpp)
file(GLOB CONTRA_TEST_HEADERS *.hpp)

rwthvr_add_executable(
  NAME contra_tests
  SOURCES ${CONTRA_TEST_SOURCES} 
  HEADERS ${CONTRA_TEST_HEADERS}
  SUPPRESS_WARNINGS_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/contra_tests/suppress_warnings.hpp"
  )
  
target_include_directories(contra_tests
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
  )
set(CONTRA_TEST_ARGUMENTS ""
  CACHE STRING
  "Arguments, e.g., tags, that are passed to contra_tests executable."
  )
add_test(NAME contra_tests
  COMMAND contra_tests ${CONTRA_TESTS_ARGUMENTS}
  )

target_link_libraries(contra_tests
  PRIVATE contra
  PRIVATE ${CONAN_OR_CMAKE_catch}
  )
