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

#include "contra/file_transport.hpp"

#include <cstring>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "contra/packet.hpp"

namespace contra {

const char FileTransport::kSignature[] = {'C', 'O', 'N', 'T', 'R', 'A'};

FileTransport::FileTransport(const std::string& filename)
    : filename_{filename} {}

void FileTransport::Send(const Packet& packet) {
  std::ofstream stream(filename_, std::fstream::binary);

  stream.write(kSignature, kSignatureLength);
  WriteSchema(packet.schema, &stream);
  WriteData(packet.data, &stream);
}

void FileTransport::WriteSchema(const std::string& schema,
                                std::ofstream* stream) const {
  const std::size_t size = schema.size();
  stream->write(reinterpret_cast<const char*>(&size), sizeof(size));
  *stream << schema;
}

void FileTransport::WriteData(const std::vector<uint8_t>& data,
                              std::ofstream* stream) const {
  const std::size_t size = data.size();
  stream->write(reinterpret_cast<const char*>(&size), sizeof(size));
  stream->write(reinterpret_cast<const char*>(data.data()),
                static_cast<std::streamsize>(data.size()));
}

std::vector<Packet> FileTransport::Receive() {
  Packet return_packet;

  std::ifstream stream(filename_, std::fstream::binary);

  if (!ReadAndCheckSignature(&stream)) {
    std::cerr << "Failed to verify signature!" << std::endl;
    return {};
  }

  return_packet.schema = ReadSchema(&stream);
  return_packet.data = ReadData(&stream);

  return {return_packet};
}

bool FileTransport::ReadAndCheckSignature(std::ifstream* stream) const {
  std::vector<char> signature_buffer(kSignatureLength + 1, 0x00);
  stream->read(signature_buffer.data(), kSignatureLength);
  return std::string(signature_buffer.data()) ==
         std::string(kSignature, kSignature + kSignatureLength);
}

std::string FileTransport::ReadSchema(std::ifstream* stream) const {
  std::streamsize size = 0u;
  stream->read(reinterpret_cast<char*>(&size), sizeof(size));

  std::vector<char> schema(static_cast<std::size_t>(size + 1), 0x00);
  stream->read(schema.data(), size);

  return std::string(schema.data());
}

std::vector<uint8_t> FileTransport::ReadData(std::ifstream* stream) const {
  std::streamsize size = 0u;
  stream->read(reinterpret_cast<char*>(&size), sizeof(size));

  std::vector<uint8_t> data(static_cast<std::size_t>(size), 0x00);
  stream->read(reinterpret_cast<char*>(data.data()), size);

  return data;
}

}  // namespace contra
