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

#include <string>
#include <vector>

#include "catch/catch.hpp"

#include "contra/file_transport.hpp"
#include "contra/packet.hpp"

#include "utilities/packet_matcher.hpp"
#include "utilities/test_data.hpp"

TEST_CASE("make sure that the involved size types have equal size",
          "[contra][contra::FileTransport]") {
  REQUIRE(sizeof(std::streamsize) == sizeof(std::size_t));
}

SCENARIO("a Packet can be transported", "[contra][contra::FileTransport][.]") {
  GIVEN("a file transport pair") {
    contra::FileTransport sender("tmp.contra");
    contra::FileTransport receiver("tmp.contra");

    WHEN("a single packet is sent and received") {
      sender.Send(test_utilities::ANY_PACKET);
      const auto received_packets = receiver.Receive();

      THEN("it arrives correctly") {
        REQUIRE(received_packets.size() == 1);
        REQUIRE_THAT(received_packets[0], Equals(test_utilities::ANY_PACKET));
      }
    }
  }
}
