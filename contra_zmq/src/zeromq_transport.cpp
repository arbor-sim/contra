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

ZMQTransport::ZMQTransport(const Type type, const std::string adress)
    : context_(1), socket_(context_, ZMQ_DEALER) {
  if (type == ZMQTransport::Type::SERVER) {
    socket_.bind(adress);
  } else if (type == ZMQTransport::Type::CLIENT) {
    socket_.connect(adress);
  }
}

void ZMQTransport::Send(const Packet& packet) {
  serialized_packet_ = SerializePacket(packet);
  auto size = sizeof(std::vector<uint8_t>) +
              (sizeof(uint8_t) * serialized_packet_.size());

  zmq::message_t message(size);
  memcpy(message.data(), &serialized_packet_, size);

  if (send_without_client_) {
    if (!socket_.send(message, ZMQ_DONTWAIT)) {
      std::cout << "WARNING: No client available! Data is Lost!" << std::endl;
    }
  } else {
    socket_.send(message);
  }
}

std::vector<Packet> ZMQTransport::Receive() {
  std::vector<Packet> packets;
  zmq::message_t received_message;

  while (socket_.recv(&received_message, ZMQ_DONTWAIT)) {
    auto message = *static_cast<std::vector<uint8_t>*>(received_message.data());
    packets.push_back(DeserializePacket(message));
    received_message.rebuild();
  }
  return packets;
}

void ZMQTransport::SetSendWithoutClient(const bool send_without_client) {
  send_without_client_ = send_without_client;
}

bool ZMQTransport::GetSendWithoutClient() const { return send_without_client_; }

}  // namespace contra
