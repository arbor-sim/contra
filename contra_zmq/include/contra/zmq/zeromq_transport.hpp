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

#ifndef CONTRA_ZMQ_INCLUDE_CONTRA_ZMQ_ZEROMQ_TRANSPORT_HPP_
#define CONTRA_ZMQ_INCLUDE_CONTRA_ZMQ_ZEROMQ_TRANSPORT_HPP_

#include <string>
#include <vector>

#include "contra/packet.hpp"
#include "zmq.hpp"

namespace contra {

class ZMQTransport {
 public:
  enum class Type { SERVER, CLIENT };

  ZMQTransport(const Type t, const std::string adress,
               bool wait_for_messages = false);
  ZMQTransport(const ZMQTransport&) = delete;
  ZMQTransport(ZMQTransport&&) = default;
  ~ZMQTransport() = default;
  ZMQTransport& operator=(const ZMQTransport&) = delete;
  ZMQTransport& operator=(ZMQTransport&&) = delete;

  void Send(const Packet& packet);
  std::vector<Packet> Receive();

  void SetWaitForMessages(const bool send_without_client);
  bool GetWaitForMessages() const;

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;

  // The idea behind this is that we need to store the messages until they are
  // sent. So far i dont know how to ask ZMQ if the data was sent. I store a
  // random amount of messages to make sure the data is not deleted before
  // sending
  std::vector<std::vector<uint8_t>> serialized_buffer_;
  unsigned int next_to_be_sent_ = 0;
  unsigned int max_buffer_size = 10;

  bool wait_for_messages_;
};

}  // namespace contra

#endif  // CONTRA_ZMQ_INCLUDE_CONTRA_ZMQ_ZEROMQ_TRANSPORT_HPP_
