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

#ifndef CONTRA_TESTS_UTILITIES_CONDUIT_DATA_HPP_
#define CONTRA_TESTS_UTILITIES_CONDUIT_DATA_HPP_

#include "conduit/conduit_node.hpp"

namespace test_utilities {

class Helpers;

void Send(const conduit::Node& node);

conduit::Node AnyNode();
conduit::Node AnotherNode();

conduit::Node AnyUpdate();
conduit::Node UpdatedNode();
conduit::Node UpdatedNodeAllZeros();

conduit::Node ADifferentNode();

static const conduit::Node ANY_NODE{AnyNode()};
static const conduit::Node ANOTHER_NODE{AnotherNode()};

static const conduit::Node ANY_UPDATE{AnyUpdate()};
static const conduit::Node UPDATED_NODE{UpdatedNode()};
static const conduit::Node UPDATED_NODE_ALL_ZEROS{UpdatedNodeAllZeros()};

static const conduit::Node A_DIFFERENT_NODE{ADifferentNode()};

static const double ANY_VALUE{4.123};

}  // namespace test_utilities

#endif  // CONTRA_TESTS_UTILITIES_CONDUIT_DATA_HPP_
