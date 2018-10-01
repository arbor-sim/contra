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

include(WarningLevels)

macro(RWTHVR_ADD_LIBRARY)
  set(options)
  set(oneValueArgs NAME SUPPRESS_WARNINGS_HEADER)
  set(multiValueArgs SOURCES HEADERS)
  cmake_parse_arguments(RWTHVR_ADD_LIBRARY_
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  add_library(${RWTHVR_ADD_LIBRARY__NAME}
    ${RWTHVR_ADD_LIBRARY__SOURCES}
    ${RWTHVR_ADD_LIBRARY__HEADERS})

  target_include_directories(${RWTHVR_ADD_LIBRARY__NAME} PUBLIC ${CONAN_INCLUDE_DIRS})
    
  get_property(RWTHVR_ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
  set_property(GLOBAL PROPERTY RWTHVR_ALL_SOURCES 
    ${RWTHVR_ALL_SOURCES}
    ${RWTHVR_ADD_LIBRARY__SOURCES}
  )

  get_property(RWTHVR_ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)
  set_property(GLOBAL PROPERTY RWTHVR_ALL_HEADERS 
    ${RWTHVR_ALL_HEADERS}
    ${RWTHVR_ADD_LIBRARY__HEADERS}
  )

  set_warning_levels_rwth(${RWTHVR_ADD_LIBRARY__NAME}
    SUPPRESS_WARNINGS_HEADER ${RWTHVR_ADD_LIBRARY__SUPPRESS_WARNINGS_HEADER}
    )

  add_test_cppcheck(TARGET ${RWTHVR_ADD_LIBRARY__NAME})
endmacro()

macro(RWTHVR_ADD_EXECUTABLE)
  set(options)
  set(oneValueArgs NAME SUPPRESS_WARNINGS_HEADER)
  set(multiValueArgs SOURCES HEADERS)
  cmake_parse_arguments(RWTHVR_ADD_EXECUTABLE_
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${RWTHVR_ADD_EXECUTABLE__NAME}
    ${RWTHVR_ADD_EXECUTABLE__SOURCES}
    ${RWTHVR_ADD_EXECUTABLE__HEADERS})

  target_include_directories(${RWTHVR_ADD_EXECUTABLE__NAME} PUBLIC ${CONAN_INCLUDE_DIRS})
    
  get_property(RWTHVR_ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
  set_property(GLOBAL PROPERTY RWTHVR_ALL_SOURCES 
    ${RWTHVR_ALL_SOURCES}
    ${RWTHVR_ADD_EXECUTABLE__SOURCES}
  )

  get_property(RWTHVR_ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)
  set_property(GLOBAL PROPERTY RWTHVR_ALL_HEADERS 
    ${RWTHVR_ALL_HEADERS}
    ${RWTHVR_ADD_EXECUTABLE__HEADERS}
  )

  set_warning_levels_rwth(${RWTHVR_ADD_EXECUTABLE__NAME}
    SUPPRESS_WARNINGS_HEADER ${RWTHVR_ADD_EXECUTABLE__SUPPRESS_WARNINGS_HEADER}
    )

  add_test_cppcheck(TARGET ${RWTHVR_ADD_EXECUTABLE__NAME})
endmacro()
