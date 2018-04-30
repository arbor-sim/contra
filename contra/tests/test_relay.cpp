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

#include "catch/catch.hpp"

#include "contra/file_transport.hpp"
#include "contra/relay.hpp"
#include "contra/shared_memory_transport.hpp"

#include "utilities/conduit_helpers.hpp"
#include "utilities/conduit_node_matcher.hpp"
#include "utilities/reset_shared_memory.hpp"

#ifdef _WIN32
namespace boost {
namespace interprocess {
namespace ipcdetail {
void get_shared_dir(std::string& shared_dir) {  // NOLINT runtime/references
  *shared_dir = ".";
}
}  // namespace ipcdetail
}  // namespace interprocess
}  // namespace boost
#endif

#define RELAY_TRANSPORT_TEST(transport_type, sender_params, receiver_params) \
  contra::Relay<transport_type> sender{sender_params};                       \
  contra::Relay<transport_type> receiver{receiver_params};                   \
                                                                             \
  sender.Send(test_utilities::ANY_NODE);                                     \
  const auto received_nodes = receiver.Receive();                            \
                                                                             \
  REQUIRE(received_nodes.size() == 1);                                       \
  REQUIRE_THAT(received_nodes[0], Equals(test_utilities::ANY_NODE));

SCENARIO("Data gets transported via FileTransport", "[contra][contra::Relay]") {
  RELAY_TRANSPORT_TEST(contra::FileTransport, "relay_file_transport.contra",
                       "relay_file_transport.contra");
}

SCENARIO("Data gets transported via SharedMemoryTransport",
         "[contra][contra::Relay]") {
  contra::SharedMemoryTransport::Destroy();

  RELAY_TRANSPORT_TEST(contra::SharedMemoryTransport,
                       contra::SharedMemoryTransport::Create(),
                       contra::SharedMemoryTransport::Access());
}
