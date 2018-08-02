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

ZMQTransport::ZMQTransport(const Type type, const std::string adress,
                           bool wait_for_messages)
    : context_(1),
      socket_(context_, ZMQ_DEALER),
      wait_for_messages_(wait_for_messages) {
  if (type == ZMQTransport::Type::SERVER) {
    socket_.bind(adress);
  } else if (type == ZMQTransport::Type::CLIENT) {
    socket_.connect(adress);
  }
}

void ZMQTransport::Send(const Packet& packet) {
  serialized_buffer_.push_back(SerializePacket(packet));
  auto size =
      sizeof(std::vector<uint8_t>) +
      (sizeof(uint8_t) * serialized_buffer_.at(next_to_be_sent_).size());

  size = sizeof(serialized_buffer_.at(next_to_be_sent_));
  zmq::message_t message(size);
  memcpy(message.data(), &serialized_buffer_.at(next_to_be_sent_), size);

  if (!wait_for_messages_) {
    if (!socket_.send(message, ZMQ_DONTWAIT)) {
      std::cout << "WARNING: No client available! Data is Lost!" << std::endl;
    }
  } else {
    socket_.send(message);
  }
  if (serialized_buffer_.size() > 10) {
    serialized_buffer_.erase(serialized_buffer_.begin());
  } else {
    next_to_be_sent_++;
  }
}

std::vector<Packet> ZMQTransport::Receive() {
  std::vector<Packet> packets;
  zmq::message_t received_message;
  if (!wait_for_messages_) {
    while (socket_.recv(&received_message, ZMQ_DONTWAIT)) {
      auto message =
          *static_cast<std::vector<uint8_t>*>(received_message.data());
      packets.push_back(DeserializePacket(message));
      received_message.rebuild();
    }
  } else {
    socket_.recv(&received_message);
    auto message = *static_cast<std::vector<uint8_t>*>(received_message.data());
    packets.push_back(DeserializePacket(message));
    received_message.rebuild();
  }
  return packets;
}

void ZMQTransport::SetWaitForMessages(const bool SetWaitForMessages) {
  wait_for_messages_ = SetWaitForMessages;
}

bool ZMQTransport::GetWaitForMessages() const { return wait_for_messages_; }

}  // namespace contra
