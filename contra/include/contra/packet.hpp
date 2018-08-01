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

#ifndef CONTRA_INCLUDE_CONTRA_PACKET_HPP_
#define CONTRA_INCLUDE_CONTRA_PACKET_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace contra {

struct Packet {
  std::string schema;
  std::vector<uint8_t> data;
};

std::vector<uint8_t> SerializePacket(const Packet& packet);
Packet SerializePacket(const std::vector<uint8_t>& data);

}  // namespace contra

#endif  // CONTRA_INCLUDE_CONTRA_PACKET_HPP_
