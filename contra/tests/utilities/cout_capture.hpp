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

#ifndef CONTRA_TESTS_UTILITIES_COUT_CAPTURE_HPP_
#define CONTRA_TESTS_UTILITIES_COUT_CAPTURE_HPP_

#include <iostream>
#include <sstream>
#include <string>

#include "catch/catch.hpp"

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

#endif  // CONTRA_TESTS_UTILITIES_COUT_CAPTURE_HPP_
