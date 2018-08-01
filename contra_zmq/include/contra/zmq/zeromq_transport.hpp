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

  ZMQTransport(const Type t, const std::string adress);
  ZMQTransport(const ZMQTransport&) = delete;
  ZMQTransport(ZMQTransport&&) = default;
  ~ZMQTransport() = default;
  ZMQTransport& operator=(const ZMQTransport&) = delete;
  ZMQTransport& operator=(ZMQTransport&&) = delete;

  void Send(const Packet& packet);
  std::vector<Packet> Receive();

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
};

}  // namespace contra

#endif  // CONTRA_ZMQ_INCLUDE_CONTRA_ZMQ_ZEROMQ_TRANSPORT_HPP_
