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

#include "contra/test_utilities/packet_matcher.hpp"
#include "contra/test_utilities/test_data.hpp"

#include "zmq.hpp"

namespace {

const std::vector<contra::Packet> NONEMPTY_PACKET_LIST{contra::Packet()};

void CreateServer() {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REP);
  socket.bind("tcp://*:5555");

  bool packet_send = false;
  while (!packet_send) {
    zmq::message_t request;

    //  Wait for next request from client
    socket.recv(&request);
    std::cout << "SERVER: Received packet request" << std::endl;

    //  Do some 'work'
    Sleep(1);

    zmq::message_t reply(sizeof(test_utilities::ANY_PACKET));

    memcpy(reply.data(), &test_utilities::ANY_PACKET,
           sizeof(test_utilities::ANY_PACKET));
    socket.send(reply);
    packet_send = true;
  }
}

contra::Packet AskForPacket() {
  //  Prepare our context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);

  std::cout << "Connecting topacket server..." << std::endl;
  socket.connect("tcp://localhost:5555");

  zmq::message_t request(5);
  memcpy(request.data(), "Hello", 5);
  std::cout << "Sending Request "
            << "..." << std::endl;
  socket.send(request);

  //  Get the reply.
  zmq::message_t received_reply;
  socket.recv(&received_reply);
  std::cout << "Received packet " << std::endl;
  contra::Packet packet_received =
      *static_cast<contra::Packet*>(received_reply.data());
  return packet_received;
}

}  // namespace

SCENARIO("Sending and receiving ", "[contra][contra::]") {
  GIVEN("A Server that Sends out packets upon requests") {
    std::thread t1(CreateServer);
    WHEN("A Client asks for a package") {
      THEN("the recieved packet matches the send one") {
        // CreateClient();

        auto received_packet = AskForPacket();
        REQUIRE_THAT(received_packet, Equals(test_utilities::ANY_PACKET));
      }
    }
    t1.join();
  }
}
