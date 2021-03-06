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

#ifndef CONTRA_INCLUDE_CONTRA_RELAY_HPP_
#define CONTRA_INCLUDE_CONTRA_RELAY_HPP_

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "conduit/conduit_node.hpp"

#include "contra/packet.hpp"

namespace contra {

template <typename Transport>
class Relay {
 public:
  template <typename... ConstructorArguments>
  explicit Relay(ConstructorArguments&&... transport_arguments);

  Relay(const Relay&) = delete;
  Relay(Relay&&) = default;
  ~Relay() = default;

  Relay& operator=(const Relay&) = delete;
  Relay& operator=(Relay&&) = delete;

  void Send(const conduit::Node& node);
  std::vector<conduit::Node> Receive();

 private:
  Packet CreatePacket(const conduit::Node& node) const;
  conduit::Node CreateNode(const Packet& packet) const;

  Transport transport_;
};

template <typename Transport>
template <typename... ConstructorArguments>
Relay<Transport>::Relay(ConstructorArguments&&... transport_arguments)
    : transport_(std::forward<ConstructorArguments>(transport_arguments)...) {
  static_assert(
      std::is_constructible<Transport, ConstructorArguments...>::value,
      "Transport must be constructible using ConstructorArguments");
}

template <typename Transport>
void Relay<Transport>::Send(const conduit::Node& node) {
  const Packet packet{CreatePacket(node)};
  transport_.Send(packet);
}

template <typename Transport>
Packet Relay<Transport>::CreatePacket(const conduit::Node& node) const {
  Packet packet;

  const conduit::Schema schema{node.schema()};
  conduit::Schema compact_schema;
  schema.compact_to(compact_schema);
  packet.schema = compact_schema.to_json();

  node.serialize(packet.data);
  return packet;
}

template <typename Transport>
std::vector<conduit::Node> Relay<Transport>::Receive() {
  const auto packets = transport_.Receive();
  std::vector<conduit::Node> nodes;
  nodes.reserve(packets.size());
  for (const auto& packet : packets) {
    nodes.push_back(CreateNode(packet));
  }
  return nodes;
}

template <typename Transport>
conduit::Node Relay<Transport>::CreateNode(const Packet& packet) const {
  constexpr bool use_external_data{false};
  return conduit::Node(
      packet.schema,
      const_cast<void*>(reinterpret_cast<const void*>(packet.data.data())),
      use_external_data);
}

}  // namespace contra

#endif  // CONTRA_INCLUDE_CONTRA_RELAY_HPP_
