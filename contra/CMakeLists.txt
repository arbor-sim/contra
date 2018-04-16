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


file(GLOB CONTRA_SOURCES src/*.cpp)

file(GLOB CONTRA_HEADERS include/contra/*.hpp)



add_library(contra ${CONTRA_SOURCES}
 ${CONTRA_HEADERS}
)


GENERATE_EXPORT_HEADER(contra EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/include/contra/export.hpp")

SetWarningLevelRWTH(contra)
