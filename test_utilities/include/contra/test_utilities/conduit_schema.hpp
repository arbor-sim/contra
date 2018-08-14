// -----------------------------------------------------------------------------
// contra -- a lightweight transport library for conduit data
// 
// Copyright (c) 2018 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
// -----------------------------------------------------------------------------
//                                  License
// 
// The license of the software changes depending on if it is compiled with or
// without ZeroMQ support. See the LICENSE file for more details.
// -----------------------------------------------------------------------------
//                          Apache License, Version 2.0
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// -----------------------------------------------------------------------------
// Contra is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Contra is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Contra.  If not, see <https://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#ifndef TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_CONDUIT_SCHEMA_HPP_
#define TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_CONDUIT_SCHEMA_HPP_

#include <sstream>
#include <string>

namespace test_utilities {
namespace conduit_schema {

template <typename T>
inline std::string OpenTag(T tag) {
  std::stringstream s;
  s << '\"' << tag << '\"' << ":{ \n";
  return s.str();
}

inline std::string CloseTag() { return std::string("} \n"); }
inline std::string CloseTagNext() { return std::string("}, \n"); }

inline std::string DoubleData(std::size_t offset) {
  std::stringstream s;
  s << "dtype:float64, ";
  s << "number_of_elements:1, ";
  s << "offset:" << offset << ", ";
  s << "stride:8, ";
  s << "element_bytes:8";
  s << "\n";
  return s.str();
}

inline void RemoveNextIndicator(std::stringstream* s) {
  s->clear();
  s->seekp(static_cast<std::streamoff>(s->str().size() - 3));
  *s << " \n";
}

}  // namespace conduit_schema
}  // namespace test_utilities

#endif  // TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_CONDUIT_SCHEMA_HPP_
