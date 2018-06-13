//------------------------------------------------------------------------------
// contra -- a lightweigth transport library for conduit data
//
// Copyright (c) 2018 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
//------------------------------------------------------------------------------
//                                  License
//
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
//------------------------------------------------------------------------------

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
