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

#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "catch2/catch.hpp"

#include "contra/zmq/zeromq_transport.hpp"

#include "contra/test_utilities/packet_matcher.hpp"
#include "contra/test_utilities/test_data.hpp"

#include "zmq.hpp"

SCENARIO("Server and client creation ", "[contra][contra::ZMQTransport]") {
  WHEN("the Client is created before the server") {
    contra::ZMQTransport client(contra::ZMQTransport::Type::CLIENT,
                                "tcp://localhost:5555");
    // Waiit to make sure server is created late enough
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    contra::ZMQTransport server(contra::ZMQTransport::Type::SERVER,
                                "tcp://*:5555");
    THEN("the connection is not established") {
      server.Send(test_utilities::ANY_PACKET);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      auto received_packets{client.Receive()};
      REQUIRE(received_packets.empty());
    }
  }
}

SCENARIO("Sending and receiving ", "[contra][contra::ZMQTransport]") {
  GIVEN("a Server and a Client") {
    contra::ZMQTransport server(contra::ZMQTransport::Type::SERVER,
                                "tcp://*:5555");
    server.SetWaitForMessages(true);
    contra::ZMQTransport client(contra::ZMQTransport::Type::CLIENT,
                                "tcp://localhost:5555");
    WHEN("Receive() is called without any packages sent") {
      THEN("received packets are epmty and no exception is thrown") {
        REQUIRE_NOTHROW(client.Receive());
        REQUIRE(client.Receive().size() == 0);
      }
    }
    WHEN("a single packet is sent and received") {
      server.Send(test_utilities::ANY_PACKET);
      auto received_packets{client.Receive()};
      while (received_packets.size() == 0) {
        received_packets = client.Receive();
      }
      THEN("the recieved packet matches the send one") {
        REQUIRE_THAT(received_packets.front(),
                     Equals(test_utilities::ANY_PACKET));
      }
      WHEN("Receive() is called again") {
        THEN("no more packets are received") {
          REQUIRE(client.Receive().size() == 0);
        }
      }
    }
    WHEN("a multiple packets are sent") {
      server.Send(test_utilities::ANY_PACKET);
      server.Send(test_utilities::ANOTHER_PACKET);
      server.Send(test_utilities::THIRD_PACKET);

      // Waiit to make sure all packets where send
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      auto received_packets{client.Receive()};
      THEN("the same amount of packages is received within 1 second") {
        REQUIRE(received_packets.size() == 3);
      }
      THEN("the packets sent match the ones received") {
        REQUIRE_THAT(received_packets.front(),
                     Equals(test_utilities::ANY_PACKET));
        REQUIRE_THAT(received_packets.at(1),
                     Equals(test_utilities::ANOTHER_PACKET));
        REQUIRE_THAT(received_packets.back(),
                     Equals(test_utilities::THIRD_PACKET));
      }
    }
  }
}
