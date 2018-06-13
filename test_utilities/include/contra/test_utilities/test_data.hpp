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

#ifndef TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_TEST_DATA_HPP_
#define TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_TEST_DATA_HPP_

#include "contra/packet.hpp"

namespace test_utilities {

static const contra::Packet ANY_PACKET{{"any string"}, {1, 2, 4, 3, 5, 6}};
static const contra::Packet ANOTHER_PACKET{{"another string"},
                                           {22, 23, 24, 26, 25}};
static const contra::Packet THIRD_PACKET{{"third string"}, {36, 38, 37}};

}  // namespace test_utilities

#endif  // TEST_UTILITIES_INCLUDE_CONTRA_TEST_UTILITIES_TEST_DATA_HPP_
