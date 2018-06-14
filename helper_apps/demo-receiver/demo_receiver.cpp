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

#include <string>
#include <thread>
#include "contra/relay.hpp"
#include "contra_boost-shmem/shared_memory_transport.hpp"

int main() {
  contra::Relay<contra::SharedMemoryTransport> relay;
  std::vector<conduit::Node> data;
  do {
    data = relay.Receive();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (data.size() == 0);

  std::cout << data[0]["foo/bar"].to_double();

  return EXIT_SUCCESS;
}
