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

#ifndef TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_COUT_CAPTURE_HPP_
#define TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_COUT_CAPTURE_HPP_

#include <iostream>
#include <sstream>
#include <string>

#include "contra_tests/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "catch2/catch.hpp"
SUPPRESS_WARNINGS_END

namespace test_utilities {

class CoutCapture {
 public:
  CoutCapture() { original_rdbuf_ = std::cout.rdbuf(cout_stream_.rdbuf()); }
  ~CoutCapture() { std::cout.rdbuf(original_rdbuf_); }

  bool operator==(const std::string &other) const {
    return cout_stream_.str() == other;
  }

  std::string ToString() const { return "\"" + cout_stream_.str() + "\""; }

 private:
  std::streambuf *original_rdbuf_;
  std::stringstream cout_stream_;
};

}  // namespace test_utilities

namespace Catch {

template <>
struct StringMaker<test_utilities::CoutCapture> {
  static std::string convert(const test_utilities::CoutCapture &cout_capture) {
    return cout_capture.ToString();
  }
};

}  // namespace Catch

#endif  // TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_COUT_CAPTURE_HPP_
