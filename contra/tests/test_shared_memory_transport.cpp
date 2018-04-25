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

#include "catch/catch.hpp"

#include "contra/shared_memory_transport.hpp"

SCENARIO("Packet shared memory creation",
         "[contra][contra::SharedMemoryTransport]") {
  GIVEN("A shared memory segment") {}
  contra::SharedMemoryTransport segment{
      contra::SharedMemoryTransport::Create()};
  WHEN("I ask it for its free size") {
    auto free_size_after_creation = segment.GetFreeSize();
    THEN("it is > 0") { REQUIRE(free_size_after_creation > 0); }
  }

  WHEN("I read data from the new segment") {
    THEN("it does not throw") { REQUIRE_NOTHROW(segment.Read()); }
    THEN("it is empty") { REQUIRE(segment.Read().empty()); }
  }
  segment.Destroy();
}
