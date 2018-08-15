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

#include "contra/packet.hpp"
#include <sstream>

#include <cstdint>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace contra {

std::vector<uint8_t> SerializePacket(const Packet& packet) {
  const std::size_t schema_size = packet.schema.size();
  const std::size_t data_size = packet.data.size();
  const std::size_t schema_size_offset = 0;
  const std::size_t schema_offset = schema_size_offset + sizeof(size_t);
  const std::size_t data_size_offset = schema_offset + schema_size;
  const std::size_t data_offset = data_size_offset + sizeof(size_t);
  const std::size_t message_size = data_offset + data_size;

  std::vector<uint8_t> message(message_size);
  std::memcpy(message.data() + schema_size_offset, &schema_size,
              sizeof(schema_size));
  std::memcpy(message.data() + schema_offset, packet.schema.c_str(),
              schema_size);
  std::memcpy(message.data() + data_size_offset, &data_size, sizeof(data_size));
  std::memcpy(message.data() + data_offset, packet.data.data(),
              sizeof(data_size));

  return message;
}

Packet DeserializePacket(const std::vector<uint8_t>& message) {
  const std::size_t schema_size_offset = 0;
  std::size_t schema_size;
  std::memcpy(&schema_size, message.data() + schema_size_offset,
              sizeof(schema_size));

  const std::size_t schema_offset = schema_size_offset + sizeof(size_t);
  const char* schema_begin =
      reinterpret_cast<const char*>(message.data() + schema_offset);
  std::string schema(schema_begin, schema_begin + schema_size);

  const std::size_t data_size_offset = schema_offset + schema_size;
  std::size_t data_size;
  std::memcpy(&data_size, message.data() + data_size_offset, sizeof(data_size));

  const std::size_t data_offset = data_size_offset + sizeof(size_t);
  const uint8_t* data_begin = message.data() + data_offset;
  std::vector<uint8_t> data(data_begin, data_begin + data_size);

  return {std::move(schema), std::move(data)};
}

}  // namespace contra
