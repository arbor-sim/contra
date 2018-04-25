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

#ifndef CONTRA_INCLUDE_CONTRA_FILE_TRANSPORT_HPP_
#define CONTRA_INCLUDE_CONTRA_FILE_TRANSPORT_HPP_

#include <string>
#include <vector>

#include "contra/packet.hpp"

namespace contra {

class FileTransport {
 public:
  explicit FileTransport(const std::string& filename);

  void Send(const Packet& packet);
  Packet Receive();

 private:
  void WriteSchema(const std::string& schema, std::ofstream* stream) const;
  void WriteData(const std::vector<uint8_t>& data, std::ofstream* stream) const;
  bool ReadAndCheckSignature(std::ifstream* stream) const;
  std::string ReadSchema(std::ifstream* stream) const;
  std::vector<uint8_t> ReadData(std::ifstream* stream) const;

  std::string filename_;

  static const unsigned int kSignatureLength{6};
  static const char kSignature[kSignatureLength];
};

}  // namespace contra

#endif  // CONTRA_INCLUDE_CONTRA_FILE_TRANSPORT_HPP_
