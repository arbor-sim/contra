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

#include <string>

#include "contra_tests/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "catch2/catch.hpp"
SUPPRESS_WARNINGS_END

#include "contra/contra.hpp"
#include "contra/packet.hpp"
#include "conduit/conduit_node.hpp"

#include "contra/test_utilities/packet_matcher.hpp"
#include "contra/test_utilities/test_data.hpp"
#include "contra/test_utilities/conduit_data.hpp"

SCENARIO("check Packet (de-)serialization", "[contra][Packet]") {
  auto data = contra::SerializePacket(test_utilities::ANY_PACKET);
  auto packet = contra::DeserializePacket(data);
  REQUIRE_THAT(packet, Equals(test_utilities::ANY_PACKET));
}

SCENARIO("check ANY_NODE (de-)serialization", "[contra][Packet]") {
  contra::Packet source_packet;

  const conduit::Node node = test_utilities::ANY_NODE;
  const conduit::Schema schema{node.schema()};
  conduit::Schema compact_schema;
  schema.compact_to(compact_schema);
  source_packet.schema = compact_schema.to_json();

  node.serialize(source_packet.data);

  auto data = contra::SerializePacket(source_packet);
  auto packet = contra::DeserializePacket(data);
  REQUIRE_THAT(packet, Equals(source_packet));
}
