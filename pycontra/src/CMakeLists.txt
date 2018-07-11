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

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.hpp)
file(GLOB PYTHON_SOURCES *.py)

set(PYCONTRA_OUTPUT_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/../pycontra
  CACHE PATH "Output path for pycontra python module"
  )

add_python_module(
  NAME _pycontra
  SOURCES ${SOURCES}
  HEADERS ${HEADERS}
  PYTHON_SOURCES ${PYTHON_SOURCES}
  INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  LINK_LIBRARIES contra contra_boost-shmem
    ${CONAN_OR_CMAKE_boost_python}
  OUTPUT_DIRECTORY ${PYCONTRA_OUTPUT_DIR}
  )