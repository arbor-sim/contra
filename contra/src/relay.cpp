//------------------------------------------------------------------------------
// contra -- a lightweigth transport library for conduit data
//
// Copyright (c) 2018 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualisation Group.
//------------------------------------------------------------------------------
//                                 License
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

#include "contra/relay.hpp"

#include <fstream>
#include <string>

#include "conduit/conduit_node.hpp"
#include "conduit/conduit_schema.hpp"

#include "contra/packet.hpp"

namespace contra {

Relay::Relay(const std::string& name) : transport_{name} {}

void Relay::Send(const conduit::Node& node) {
  const Packet packet{CreatePacket(node)};
  transport_.Send(packet);
}

Packet Relay::CreatePacket(const conduit::Node& node) const {
  Packet packet;

  const conduit::Schema schema{node.schema()};
  conduit::Schema compact_schema;
  schema.compact_to(compact_schema);
  packet.schema = compact_schema.to_json();

  node.serialize(packet.data);
  return packet;
}

conduit::Node Relay::Receive() {
  Packet packet{transport_.Receive()};
  return CreateNode(packet);
}

conduit::Node Relay::CreateNode(const Packet& packet) const {
  constexpr bool use_external_data{false};
  return conduit::Node(
      packet.schema,
      const_cast<void*>(reinterpret_cast<const void*>(packet.data.data())),
      use_external_data);
}

}  // namespace contra
