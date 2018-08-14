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

set(WARNING_LEVELS_RWTH_CLANG
  -Werror
  -Weverything
  -pedantic
  -pedantic-errors
  -Wextra-tokens
  -Wambiguous-member-template
  -Wbind-to-temporary-copy
  -Wno-c++98-compat
  -Wno-c++98-compat-pedantic
  -Wno-weak-vtables
  -Wno-exit-time-destructors
  -Wno-global-constructors
  -Wno-float-equal
  -Wno-shadow-field-in-constructor
)

set(WARNING_LEVELS_RWTH_GCC
  -Werror
  -Wall
  -Wextra
  -Wmissing-braces
  -pedantic
  -pedantic-errors
  -Wno-c++98-compat
)

set(WARNING_LEVELS_RWTH_MSVC
  -D_SCL_SECURE_NO_WARNINGS
  -WX
  -W4
  "$<$<CXX_COMPILER_ID:MSVC>:/w34062>"
  "$<$<CXX_COMPILER_ID:MSVC>:/w24302>"
  "$<$<CXX_COMPILER_ID:MSVC>:/w14311>"
  "$<$<CXX_COMPILER_ID:MSVC>:/w14312>"
)

# compiler identification
if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND
    CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(IS_CLANG 1)
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC" AND
    CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(IS_MSVC 1)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU" AND
    CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(IS_GCC 1)
endif()

# warning level setter macros
macro(SET_WARNING_LEVELS_RWTH TARGET)
  set(options)
  set(oneValueArgs SUPPRESS_WARNINGS_HEADER)
  set(multiValueArgs)
  cmake_parse_arguments(SWLR
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (SWLR_SUPPRESS_WARNINGS_HEADER)
    set(SUPPRESS_WARNING_HEADER_FILE ${SWLR_SUPPRESS_WARNINGS_HEADER})
  else()
    set(SUPPRESS_WARNING_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/${target}/suppress_warnings.hpp) 
  endif()
  
  find_file(SUPPRESS_WARNING_HEADER_TEMPLATE suppress_warnings.hpp.in
    NO_DEFAULT_PATH
    PATHS ${CMAKE_MODULE_PATH}
  )
  configure_file(${SUPPRESS_WARNING_HEADER_TEMPLATE} ${SUPPRESS_WARNING_HEADER_FILE})
  
  if(IS_CLANG)
    target_compile_options(${TARGET} PRIVATE ${WARNING_LEVELS_RWTH_CLANG})
  elseif(IS_GCC)
    target_compile_options(${TARGET} PRIVATE ${WARNING_LEVELS_RWTH_GCC})
  elseif(IS_MSVC)
    target_compile_options(${TARGET} PRIVATE ${WARNING_LEVELS_RWTH_MSVC})
  else()
    message(WARNING "SET_WARNING_LEVELS_RWTH not implemented for your compiler. "
      "Implement it to support our no-warning policy.")
  endif()
endmacro()
  
