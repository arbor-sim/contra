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

#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "catch/catch.hpp"

#include "contra/zmq/zeromq_transport.hpp"

#include "contra/test_utilities/packet_matcher.hpp"
#include "contra/test_utilities/test_data.hpp"

#include "zmq.hpp"

SCENARIO("Server and client creation ", "[contra][contra::ZMQTransport]") {
  WHEN("the Client is created before the server") {
    THEN("it does not throw an error") {
      REQUIRE_NOTHROW(contra::ZMQTransport(contra::ZMQTransport::Type::CLIENT,
                                           "tcp://localhost:5555"));
      REQUIRE_NOTHROW(contra::ZMQTransport(contra::ZMQTransport::Type::SERVER,
                                           "tcp://*:5555"));
    }
  }
}

//SCENARIO("Sending before connecting ", "[contra][contra::ZMQTransport]") {
//  GIVEN("a Server") {
//    contra::ZMQTransport server(contra::ZMQTransport::Type::SERVER,
//                                "tcp://*:5555");
//
//    contra::ZMQTransport client(contra::ZMQTransport::Type::CLIENT,
//                                "tcp://localhost:5555");
//  }
//}

SCENARIO("Sending and receiving ", "[contra][contra::ZMQTransport]") {
  GIVEN("a Server and a Client") {
    contra::ZMQTransport server(contra::ZMQTransport::Type::SERVER,
                                "tcp://*:5555");
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
