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

#include <iostream>
#include <vector>

#include "catch/catch.hpp"

#include "contra/shared_memory_transport.hpp"
#include "utilities/reset_shared_memory.hpp"

namespace {

const std::vector<contra::Packet> NONEMPTY_PACKET_LIST{contra::Packet()};

}  // namespace

SCENARIO("Packet shared memory creation",
         "[contra][contra::SharedMemoryTransport]") {
  test_utilities::ResetSharedMemory();

  GIVEN("A shared memory segment") {
    contra::SharedMemoryTransport segment{
        contra::SharedMemoryTransport::Create()};

    WHEN("I ask it for its free size") {
      const auto free_size_after_creation = segment.GetFreeSize();
      THEN("it is > 0") { REQUIRE(free_size_after_creation > 0); }
    }

    WHEN("I read data from the new segment") {
      std::vector<contra::Packet> received_packets{::NONEMPTY_PACKET_LIST};
      auto do_read = [&]() { received_packets = segment.Read(); };

      THEN("it does not throw and is empty") {
        REQUIRE_NOTHROW(do_read());
        REQUIRE(received_packets.empty());
      }
    }

    WHEN("I request a second shared memory segment") {
      const auto create_second_segment = []() {
        contra::SharedMemoryTransport segment2{
            contra::SharedMemoryTransport::Create()};
      };
      THEN("It throws an exception") {
        REQUIRE_THROWS(create_second_segment());
      }
    }

    segment.Destroy();
  }
}

SCENARIO("Packet shared memory access",
         "[contra][contra::SharedMemoryTransport]") {
  test_utilities::ResetSharedMemory();

  GIVEN("No shared memory segment") {
    THEN("Creating a shared memory access throws an exception.") {
      REQUIRE_THROWS(contra::SharedMemoryTransport{
          contra::SharedMemoryTransport::Access()});
    }
  }

  GIVEN("A shared memory segment") {
    contra::SharedMemoryTransport segment{
        contra::SharedMemoryTransport::Create()};

    THEN("Creating a shared memory access does not throw an exception.") {
      REQUIRE_NOTHROW(contra::SharedMemoryTransport{
          contra::SharedMemoryTransport::Access()});
    }

    segment.Destroy();
  }
}
