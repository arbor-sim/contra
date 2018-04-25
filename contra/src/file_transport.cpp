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

#include "contra/file_transport.hpp"

#include <cstring>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "contra/packet.hpp"

namespace contra {

FileTransport::FileTransport(const std::string& filename)
    : filename_{filename} {}

void FileTransport::Send(const Packet& /*packet*/) {
  std::ofstream stream(filename_, std::fstream::binary);
  stream << kSignature;
}

Packet FileTransport::Receive() {
  Packet return_packet;

  std::ifstream stream(filename_, std::fstream::binary);
  char signature_buffer[kSignatureLength + 1]{};
  std::fill_n(signature_buffer, kSignatureLength + 1, 0x00);
  stream.read(signature_buffer, kSignatureLength);

  if (std::string(signature_buffer) == std::string(kSignature)) {
    return_packet =
        Packet{std::string{"Foo"}, std::vector<uint8_t>{0x01u, 0x03u, 0x02u}};
  }

  return return_packet;
}

}  // namespace contra
