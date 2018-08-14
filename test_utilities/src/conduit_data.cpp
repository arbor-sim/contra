// -----------------------------------------------------------------------------
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

#include "contra/test_utilities/conduit_data.hpp"

namespace test_utilities {

conduit::Node AnyNode() {
  conduit::Node node;
  node["A/B/C"] = 3.1415;
  node["A/B/D"] = 4.124;
  node["A/E"] = 42.0;
  return node;
}

conduit::Node AnotherNode() {
  conduit::Node node;
  node["A/B/C"] = 2.0 * 3.1415;
  node["A/B/D"] = 3.0 * 4.124;
  node["A/E"] = 4.0 * 42.0;
  return node;
}

conduit::Node AnyUpdate() {
  conduit::Node node;
  node["A/B/F"] = 2.0 * 3.1415;
  node["A/B/G"] = 3.0 * 4.124;
  node["A/H"] = 4.0 * 42.0;
  return node;
}

conduit::Node UpdatedNode() {
  conduit::Node node;
  node["A/B/C"] = 3.1415;
  node["A/B/D"] = 4.124;
  node["A/E"] = 42.0;
  node["A/B/F"] = 2.0 * 3.1415;
  node["A/B/G"] = 3.0 * 4.124;
  node["A/H"] = 4.0 * 42.0;
  return node;
}

conduit::Node UpdatedNodeAllZeros() {
  conduit::Node node;
  node["A/B/C"] = 0.0;
  node["A/B/D"] = 0.0;
  node["A/E"] = 0.0;
  node["A/B/F"] = 0.0;
  node["A/B/G"] = 0.0;
  node["A/H"] = 0.0;
  return node;
}

conduit::Node ADifferentNode() { return AnyUpdate(); }

}  // namespace test_utilities
