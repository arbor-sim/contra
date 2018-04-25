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

#include "contra/packet.hpp"

namespace contra {

class FileTransport {
 public:
  explicit FileTransport(const std::string& filename);

  void Send(const Packet& packet);
  Packet Receive();

 private:
  bool ReadAndCheckSignature(std::ifstream* stream) const;

  std::string filename_;

  constexpr static char kSignature[]{"CONTRA"};
  constexpr static unsigned int kSignatureLength{6};
};

}  // namespace contra

#endif  // CONTRA_INCLUDE_CONTRA_FILE_TRANSPORT_HPP_
