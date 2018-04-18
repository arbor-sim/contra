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
   
  get_property(ALL_FILES GLOBAL PROPERTY CONTRA_ALL_FILES)
  set_property(GLOBAL PROPERTY CONTRA_ALL_FILES 
    ${ALL_FILES}
    ${RWTHVR_ADD_LIBRARY__SOURCES}
    ${RWTHVR_ADD_LIBRARY__HEADERS}
  )

  set_warning_levels_rwth(${RWTHVR_ADD_LIBRARY__NAME}
    SUPPRESS_WARNINGS_HEADER ${RWTHVR_ADD_LIBRARY__SUPPRESS_WARNINGS_HEADER}
    )
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
    
  get_property(ALL_FILES GLOBAL PROPERTY CONTRA_ALL_FILES)
  set_property(GLOBAL PROPERTY CONTRA_ALL_FILES 
    ${ALL_FILES}
    ${RWTHVR_ADD_EXECUTABLE__SOURCES}
    ${RWTHVR_ADD_EXECUTABLE__HEADERS}
  )

  set_warning_levels_rwth(${RWTHVR_ADD_EXECUTABLE__NAME}
    SUPPRESS_WARNINGS_HEADER ${RWTHVR_ADD_EXECUTABLE__SUPPRESS_WARNINGS_HEADER}
    )
endmacro()
