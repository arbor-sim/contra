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

function(get_target_include_directories VAR)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs)
  cmake_parse_arguments(ARGS
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (TARGET ${ARGS_TARGET})
    # message("${ARGS_TARGET}")
    get_target_property(IS_IMPORTED ${ARGS_TARGET} IMPORTED)
    # message("imported: ${IS_IMPORTED}")
    if (${IS_IMPORTED})
      # get_target_property(INCLUDE_DIRECTORIES ${ARGS_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    else (${IS_IMPORTED})
      get_target_property(INCLUDE_DIRECTORIES ${ARGS_TARGET} INCLUDE_DIRECTORIES)
      get_target_property(LINK_LIBRARIES ${ARGS_TARGET} LINK_LIBRARIES)

      foreach(LIB ${LINK_LIBRARIES})
        get_target_include_directories(LIB_INCLUDE_DIRECTORIES TARGET ${LIB})
        if (${LIB_INCLUDE_DIRECTORIES})
          set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${LIB_INCLUDE_DIRECTORIES})
        endif (${LIB_INCLUDE_DIRECTORIES})
      endforeach(LIB ${LINK_LIBRARIES})
    endif(${IS_IMPORTED})

  else (TARGET ${ARGS_TARGET})
    set(INCLUDE_DIRECTORIES "")
  endif (TARGET ${ARGS_TARGET})

  if (${INCLUDE_DIRECTORIES})
    set(${VAR} ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
  endif (${INCLUDE_DIRECTORIES})

endfunction(get_target_include_directories)