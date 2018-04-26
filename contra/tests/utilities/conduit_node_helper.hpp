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

#ifndef CONTRA_TESTS_UTILITIES_CONDUIT_NODE_HELPER_HPP_
#define CONTRA_TESTS_UTILITIES_CONDUIT_NODE_HELPER_HPP_

#include <string>

#include "catch/catch.hpp"

#include "conduit/conduit_node.hpp"

namespace Catch {
template <>
struct StringMaker<conduit::Node> {
  static std::string convert(const conduit::Node& node) {
    return node.to_json();
  }
};

namespace Matchers {

class ConduitNodeEquals : public Catch::MatcherBase<conduit::Node> {
 public:
  explicit ConduitNodeEquals(const conduit::Node& node) : node_{node} {}
  bool match(const conduit::Node& node) const override {
    return Catch::Matchers::Equals(node.to_json()).match(node_.to_json());
  }
  std::string describe() const override {
    return Catch::Matchers::Equals(node_.to_json()).describe();
  }

 private:
  const conduit::Node& node_;
};

inline ConduitNodeEquals Equals(const conduit::Node& node) {
  return ConduitNodeEquals(node);
}

}  // namespace Matchers

}  // namespace Catch

using Catch::Matchers::Equals;

#endif  // CONTRA_TESTS_UTILITIES_CONDUIT_NODE_HELPER_HPP_
