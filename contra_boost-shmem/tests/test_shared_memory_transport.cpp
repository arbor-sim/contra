// -- /--------------------------------------------------------------------------
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

#include <chrono>
#include <random>
#include <thread>
#include <vector>

#include "catch/catch.hpp"

#include "contra/boost-shmem/shared_memory_transport.hpp"

#include "contra/test_utilities/packet_matcher.hpp"
#include "contra/test_utilities/test_data.hpp"

namespace {

const std::vector<contra::Packet> NONEMPTY_PACKET_LIST{contra::Packet()};

}  // namespace

SCENARIO("Packet shared memory creation",
         "[contra][contra::SharedMemoryTransport]") {
  contra::SharedMemoryTransport::Destroy();

  GIVEN("A shared memory segment") {
    contra::SharedMemoryTransport segment;

    WHEN("I ask it for its free size") {
      const auto free_size_after_creation = segment.GetFreeSize();
      THEN("it is > 0") { REQUIRE(free_size_after_creation > 0); }
    }

    WHEN("I read data from the new segment") {
      std::vector<contra::Packet> received_packets{::NONEMPTY_PACKET_LIST};
      auto do_read = [&]() { received_packets = segment.Receive(); };

      THEN("it does not throw and is empty") {
        REQUIRE_NOTHROW(do_read());
        REQUIRE(received_packets.empty());
      }
    }
  }
}

SCENARIO("Packet shared memory access",
         "[contra][contra::SharedMemoryTransport]") {
  contra::SharedMemoryTransport::Destroy();

  GIVEN("A shared memory segment") {
    contra::SharedMemoryTransport segment_create;

    THEN("Creating a shared memory access does not throw an exception.") {
      REQUIRE_NOTHROW(contra::SharedMemoryTransport());
    }
  }
}

SCENARIO("Data gets transported through shared memory",
         "[contra][contra::SharedMemoryTransport]") {
  contra::SharedMemoryTransport::Destroy();

  GIVEN("A shared memory segment and access") {
    contra::SharedMemoryTransport segment_create;
    contra::SharedMemoryTransport segment_access;

    WHEN("a single packet is sent and received") {
      segment_create.Send(test_utilities::ANY_PACKET);
      auto received_packets{segment_access.Receive()};
      THEN("it arrives correctly") {
        REQUIRE(received_packets.size() == 1);
        REQUIRE_THAT(received_packets.front(),
                     Equals(test_utilities::ANY_PACKET));
      }
      THEN("the shared memory is empty") {
        REQUIRE(segment_access.Receive().empty());
      }
    }

    WHEN("Multiple Packets are sent and received") {
      segment_create.Send(test_utilities::ANY_PACKET);
      segment_create.Send(test_utilities::ANOTHER_PACKET);
      segment_create.Send(test_utilities::THIRD_PACKET);

      auto received_packets{segment_access.Receive()};
      THEN("they arrive correctly") {
        REQUIRE(received_packets.size() == 3);
        REQUIRE_THAT(received_packets[0], Equals(test_utilities::ANY_PACKET));
        REQUIRE_THAT(received_packets[1],
                     Equals(test_utilities::ANOTHER_PACKET));
        REQUIRE_THAT(received_packets[2], Equals(test_utilities::THIRD_PACKET));
      }
      THEN("the shared memory is empty") {
        REQUIRE(segment_access.Receive().empty());
      }
    }
  }
}

namespace {

void Send(contra::SharedMemoryTransport* transport) {
  std::random_device random_seed;
  std::mt19937 generator(random_seed());
  std::uniform_int_distribution<> distribution(1, 4);  // define the range

  for (auto i = 0u; i < 10; ++i) {
    const int wait = distribution(generator);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));
    transport->Send(test_utilities::ANY_PACKET);
  }
}

void Receive(contra::SharedMemoryTransport* transport) {
  std::random_device random_seed;
  std::mt19937 generator(random_seed());
  std::uniform_int_distribution<> distribution(3, 6);  // define the range

  for (auto i = 0u; i < 10; ++i) {
    const int wait = distribution(generator);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));
    transport->Receive();
  }
}

constexpr bool we_reach_this_before_timeout = true;

}  // namespace

SCENARIO("Synchronization across separate threads does not accidently block",
         "[niv][niv::RelaySharedMemory]") {
  contra::SharedMemoryTransport::Destroy();

  GIVEN("a pair of shared memory transports") {
    contra::SharedMemoryTransport segment_create;
    contra::SharedMemoryTransport segment_access;

    WHEN("these send and receive in separate threads") {
      std::thread sender(::Send, &segment_access);
      std::thread receiver(::Receive, &segment_create);
      THEN("they do not block each other") {
        sender.join();
        receiver.join();
        REQUIRE(::we_reach_this_before_timeout);
      }
    }
  }
}

TEST_CASE("reference counting", "[niv][niv::RelaySharedMemory]") {
  contra::SharedMemoryTransport::Destroy();

  // scope A
  {
    contra::SharedMemoryTransport segment_create;
    REQUIRE(segment_create.GetReferenceCount() == 1);

    // scope A.1
    {
      contra::SharedMemoryTransport segment_access;
      REQUIRE(segment_access.GetReferenceCount() == 2);
    }  // scope  A.1

    REQUIRE(segment_create.GetReferenceCount() == 1);
  }  // scope A

  // scope B
  {
    contra::SharedMemoryTransport segment_create;
    REQUIRE(segment_create.GetReferenceCount() == 1);
  }  // scope B
}
