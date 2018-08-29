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

function(ADD_PYTHON_MODULE)
  # parse arguments
  set(options )
  set(oneValueArgs NAME OUTPUT_DIRECTORY)
  set(multiValueArgs
    SOURCES HEADERS PYTHON_SOURCES INCLUDE_DIRECTORIES LINK_LIBRARIES)
  cmake_parse_arguments(ADD_PYTHON_MODULE
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  python_add_module(${ADD_PYTHON_MODULE_NAME}
    ${ADD_PYTHON_MODULE_SOURCES}
    ${ADD_PYTHON_MODULE_HEADERS}
    )

  target_include_directories(${ADD_PYTHON_MODULE_NAME}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${PYTHON_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ADD_PYTHON_MODULE_INCLUDE_DIRECTORIES}
    )

  target_link_libraries(${ADD_PYTHON_MODULE_NAME}
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${ADD_PYTHON_MODULE_LINK_LIBRARIES}
    )

  set_warning_levels_RWTH(${ADD_PYTHON_MODULE_NAME}
    SUPPRESS_WARNINGS_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/pycontra/suppress_warnings.hpp
    )
  
   
  get_property(RWTHVR_ALL_SOURCES GLOBAL PROPERTY RWTHVR_ALL_SOURCES)
  set_property(GLOBAL PROPERTY RWTHVR_ALL_SOURCES 
    ${RWTHVR_ALL_SOURCES}
    ${ADD_PYTHON_MODULE_SOURCES}
  )

  get_property(RWTHVR_ALL_HEADERS GLOBAL PROPERTY RWTHVR_ALL_HEADERS)
  set_property(GLOBAL PROPERTY RWTHVR_ALL_HEADERS 
    ${RWTHVR_ALL_HEADERS}
    ${ADD_PYTHON_MODULE_HEADERS}
  )

  add_custom_command(TARGET ${ADD_PYTHON_MODULE_NAME}
    POST_BUILD
    #OUTPUT ${ADD_PYTHON_MODULE_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${ADD_PYTHON_MODULE_NAME}>
    #DEPENDS $<TARGET_FILE:${ADD_PYTHON_MODULE_NAME}>
    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    $<TARGET_FILE:${ADD_PYTHON_MODULE_NAME}>
    ${ADD_PYTHON_MODULE_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${ADD_PYTHON_MODULE_NAME}>
    )

  foreach (ADD_PYTHON_MODULE_PYTHON_INPUT ${ADD_PYTHON_MODULE_PYTHON_SOURCES})
    get_filename_component(ADD_PYTHON_MODULE_PYTHON_INPUT_NAME
      ${ADD_PYTHON_MODULE_PYTHON_INPUT}
      NAME)
    set(ADD_PYTHON_MODULE_PYTHON_OUTPUT
      "${ADD_PYTHON_MODULE_OUTPUT_DIRECTORY}/${ADD_PYTHON_MODULE_PYTHON_INPUT_NAME}"
      )
    set(ADD_PYTHON_MODULE_CUSTOM_TARGET_NAME
      "${ADD_PYTHON_MODULE_NAME}_COPY_${ADD_PYTHON_MODULE_PYTHON_INPUT_NAME}"
      )
    add_custom_command(
      OUTPUT ${ADD_PYTHON_MODULE_PYTHON_OUTPUT}
      DEPENDS ${ADD_PYTHON_MODULE_PYTHON_INPUT}
      COMMAND ${CMAKE_COMMAND}
      -E copy_if_different 
      ${ADD_PYTHON_MODULE_PYTHON_INPUT}
      ${ADD_PYTHON_MODULE_PYTHON_OUTPUT}
      )
    add_custom_target(${ADD_PYTHON_MODULE_CUSTOM_TARGET_NAME}
      DEPENDS
      ${ADD_PYTHON_MODULE_PYTHON_OUTPUT}
      )
    add_dependencies(${ADD_PYTHON_MODULE_NAME}
      ${ADD_PYTHON_MODULE_CUSTOM_TARGET_NAME}
      )
  endforeach(ADD_PYTHON_MODULE_PYTHON_INPUT)
endfunction()
