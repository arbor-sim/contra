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

file(GLOB CONTRA_SOURCES src/*.cpp)
file(GLOB CONTRA_HEADERS include/contra/boost-shmem/*.hpp)

rwthvr_add_library(NAME contra_boost-shmem
  SOURCES ${CONTRA_SOURCES}
  HEADERS ${CONTRA_HEADERS}
  SUPPRESS_WARNINGS_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/contra/boost-shmem/suppress_warnings.hpp"
  )
generate_export_header(contra_boost-shmem
  EXPORT_FILE_NAME
  "${CMAKE_CURRENT_BINARY_DIR}/include/contra/boost-shmem/export.hpp"
  )
  
target_include_directories(contra_boost-shmem
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
  )
  
target_link_libraries(contra_boost-shmem
  PUBLIC contra
  PUBLIC ${CONAN_OR_CMAKE_boost_interprocess}
  PUBLIC ${CONAN_OR_CMAKE_conduit}
  )

if (NOT MSVC)
  target_compile_options(contra_boost-shmem
    PRIVATE -fPIC
    )
endif (NOT MSVC)

add_subdirectory(tests)
