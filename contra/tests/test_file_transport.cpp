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

#include <string>
#include <vector>

#include "catch/catch.hpp"

#include "contra/file_transport.hpp"
#include "contra/packet.hpp"
#include "utilities/cout_capture.hpp"

SCENARIO("a Packet can be transported", "[contra][contra::FileTransport]") {
  REQUIRE(sizeof(std::streamsize) == sizeof(std::size_t));

  const std::string any_string{"Foo"};
  const std::vector<uint8_t> any_data{0x01u, 0x03u, 0x02u};

  contra::FileTransport transport("tmp.contra");
  transport.Send(contra::Packet{any_string, any_data});

  auto received = transport.Receive();

  REQUIRE(received.schema == any_string);
  REQUIRE(received.data == any_data);
  REQUIRE(true == false);
}
