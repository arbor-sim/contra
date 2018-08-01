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

#include "contra/zmq/zeromq_transport.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "zmq.hpp"

namespace contra {

ZMQTransport::ZMQTransport(Type type, std::string adress)
    : context_(1), socket_(context_, ZMQ_DEALER) {
  if (type == ZMQTransport::Type::SERVER) {
    socket_.bind(adress);
  } else if (type == ZMQTransport::Type::CLIENT) {
    socket_.connect(adress);
  }
}

void ZMQTransport::Send(const Packet& packet) {
  zmq::message_t message(sizeof(packet));
  memcpy(message.data(), &packet, sizeof(packet));
  socket_.send(message);
}

std::vector<Packet> ZMQTransport::Receive() {
  std::vector<Packet> packets;
  zmq::message_t received_message;

  while (socket_.recv(&received_message, ZMQ_DONTWAIT)) {
    packets.push_back(*static_cast<contra::Packet*>(received_message.data()));
    received_message.rebuild();
  }

  return packets;
}

}  // namespace contra
