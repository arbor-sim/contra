#------------------------------------------------------------------------------
# contra
#
# Copyright (c) 2018 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
#------------------------------------------------------------------------------
#                                 License
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#------------------------------------------------------------------------------

file(GLOB CONTRA_TEST_SOURCES src/*.cpp)
file(GLOB CONTRA_TEST_HEADERS src/*.hpp)

add_executable(contra_tests
  ${CONTRA_TEST_SOURCES} 
  ${CONTRA_TEST_SOURCES}
  )
set(CONTRA_TEST_ARGUMENTS ""
  CACHE STRING
  "Arguments, e.g., tags, that are passed to contra_tests executable.")
add_test(NAME contra_tests COMMAND contra_tests ${CONTRA_TESTS_ARGUMENTS})

# --- settings ---
SetWarningLevelRWTH(contra_tests)


# --- dependencies ---
# qosp
target_include_directories(contra_tests PRIVATE contra)
target_link_libraries(contra_tests contra)
