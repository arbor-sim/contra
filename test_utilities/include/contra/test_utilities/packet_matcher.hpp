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

#ifndef TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_PACKET_MATCHER_HPP_
#define TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_PACKET_MATCHER_HPP_

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "catch/catch.hpp"

#include "contra/packet.hpp"

namespace Catch {
template <>
struct StringMaker<contra::Packet> {
  static std::string convert(const contra::Packet& packet) {
    std::stringstream str;
    str << packet.schema << '\n';
    for (auto c : packet.data) {
      str << std::setfill('0') << std::setw(2) << std::hex << uint16_t{c}
          << ' ';
    }
    str << '\n';
    return str.str();
  }
};

namespace Matchers {

class PacketEquals : public Catch::MatcherBase<contra::Packet> {
 public:
  explicit PacketEquals(const contra::Packet& packet) : packet_{packet} {}
  bool match(const contra::Packet& other) const override {
    const std::string others = StringMaker<contra::Packet>::convert(other);
    const std::string mine = StringMaker<contra::Packet>::convert(packet_);
    return Catch::Matchers::Equals(others).match(mine);
  }
  std::string describe() const override {
    return Catch::Matchers::Equals(
               StringMaker<contra::Packet>::convert(packet_))
        .describe();
  }

 private:
  const contra::Packet packet_;
};

inline PacketEquals Equals(const contra::Packet& packet) {
  return PacketEquals(packet);
}

}  // namespace Matchers

}  // namespace Catch

using Catch::Matchers::Equals;

#endif  // TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_PACKET_MATCHER_HPP_
