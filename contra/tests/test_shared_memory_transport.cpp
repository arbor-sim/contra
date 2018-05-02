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

#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "catch/catch.hpp"

#include "contra/log_time.hpp"
#include "contra/shared_memory_transport.hpp"

#include "utilities/packet_matcher.hpp"
#include "utilities/reset_shared_memory.hpp"
#include "utilities/test_data.hpp"

namespace {

const std::vector<contra::Packet> NONEMPTY_PACKET_LIST{contra::Packet()};

}  // namespace

SCENARIO("Packet shared memory creation",
         "[contra][contra::SharedMemoryTransport]") {
  CONTRA_LOG_HLINE;
  CONTRA_LOG_TIME_BEGIN("SCENARIO Packet shared memory creation");

  contra::SharedMemoryTransport::Destroy();

  CONTRA_LOG_TIME("after Destroy()");

  GIVEN("A shared memory segment") {
    contra::SharedMemoryTransport segment{
        contra::SharedMemoryTransport::Create()};

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
  CONTRA_LOG_TIME("end");
  CONTRA_LOG_HLINE;
}

SCENARIO("Packet shared memory access",
         "[contra][contra::SharedMemoryTransport]") {
  CONTRA_LOG_HLINE;
  CONTRA_LOG_TIME_BEGIN("SCENARIO Packet shared memory access");

  contra::SharedMemoryTransport::Destroy();

  CONTRA_LOG_TIME("after Destroy()");

  CONTRA_LOG_TIME("before GIVEN No shared memory segment");
  GIVEN("No shared memory segment") {
    CONTRA_LOG_TIME("GIVEN No shared memory segment");
    THEN("Creating a shared memory access throws an exception.") {
      CONTRA_LOG_TIME(
          "THEN Creating a shared memory access throws an exception.");
      CONTRA_LOG_TIME("before REQUIRE_THROWS");
      REQUIRE_THROWS(contra::SharedMemoryTransport{
          contra::SharedMemoryTransport::Access()});
      CONTRA_LOG_TIME("after REQUIRE_THROWS");
      CONTRA_LOG_TIME(
          "end THEN Creating a shared memory access throws an exception.");
    }
    CONTRA_LOG_TIME("end GIVEN No shared memory segment");
  }
  CONTRA_LOG_TIME("after GIVEN No shared memory segment");

  CONTRA_LOG_TIME("before GIVEN A shared memory segment");
  GIVEN("A shared memory segment") {
    CONTRA_LOG_TIME("GIVEN A shared memory segment");

    contra::SharedMemoryTransport segment_create{
        contra::SharedMemoryTransport::Create()};

    CONTRA_LOG_TIME(
        "before THEN Creating a shared memory access does not throw an "
        "exception.");
    THEN("Creating a shared memory access does not throw an exception.") {
      CONTRA_LOG_TIME(
          "THEN Creating a shared memory access does not throw an exception.");
      CONTRA_LOG_TIME("before REQUIRE_NOTHROW");
      REQUIRE_NOTHROW(contra::SharedMemoryTransport{
          contra::SharedMemoryTransport::Access()});
      CONTRA_LOG_TIME("after REQUIRE_THROWS");
      CONTRA_LOG_TIME(
          "end THEN Creating a shared memory access does not throw an "
          "exception.");
    }
    CONTRA_LOG_TIME("end GIVEN A shared memory segment");
  }
  CONTRA_LOG_TIME("after GIVEN A shared memory segment");

  CONTRA_LOG_TIME("end");
  CONTRA_LOG_HLINE;
  REQUIRE(true == false);
}

SCENARIO("Data gets transported through shared memory",
         "[contra][contra::SharedMemoryTransport]") {
  CONTRA_LOG_HLINE;
  CONTRA_LOG_TIME_BEGIN("SCENARIO Data gets transported through shared memory");

  contra::SharedMemoryTransport::Destroy();
  CONTRA_LOG_TIME("after Destroy()");

  GIVEN("A shared memory segment and access") {
    contra::SharedMemoryTransport segment_create{
        contra::SharedMemoryTransport::Create()};
    contra::SharedMemoryTransport segment_access{
        contra::SharedMemoryTransport::Access()};

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

    segment_create.Destroy();
  }
  CONTRA_LOG_TIME("end");
  CONTRA_LOG_HLINE;
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
         "[niv][niv::RelaySharedMemory][.]") {
  contra::SharedMemoryTransport::Destroy();

  GIVEN("a pair of shared memory transports") {
    contra::SharedMemoryTransport segment_create{
        contra::SharedMemoryTransport::Create()};
    contra::SharedMemoryTransport segment_access{
        contra::SharedMemoryTransport::Access()};

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
