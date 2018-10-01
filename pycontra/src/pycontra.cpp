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

#include <pyconfig.h>

#include "pycontra.hpp"

#include "contra/contra.hpp"
#include "contra/file_transport.hpp"
#include "contra/relay.hpp"

#ifdef WITH_SHARED_MEMORY
#include "contra/boost-shmem/shared_memory_transport.hpp"
#endif  // WITH_ZEROMQ

#ifdef WITH_ZEROMQ
#include "contra/zmq/zeromq_transport.hpp"
#endif  // WITH_ZEROMQ

#include "pycontra/suppress_warnings.hpp"

namespace pycontra {

SUPPRESS_WARNINGS_BEGIN
conduit::Node AnyNode() {
  conduit::Node node;
  node["A/B/C"] = 3.1415;
  node["A/B/D"] = 4.124;
  node["A/E"] = 42.0;
  return node;
}
SUPPRESS_WARNINGS_END

extern template void expose<contra::Relay<contra::FileTransport>>();
#ifdef WITH_SHARED_MEMORY
extern template void expose<contra::Relay<contra::SharedMemoryTransport>>();
#endif  // WITH_SHARED_MEMORY
#ifdef WITH_ZEROMQ
extern template void expose<contra::ZMQTransport>();
#endif  // WITH_ZEROMQ

SUPPRESS_WARNINGS_BEGIN
// cppcheck-suppress unusedFunction
BOOST_PYTHON_MODULE(_pycontra) {
  def("Greet", contra::Greet);
  expose<contra::Relay<contra::FileTransport>>();
#ifdef WITH_SHARED_MEMORY
  expose<contra::Relay<contra::SharedMemoryTransport>>();
#endif  // WITH_SHARED_MEMORY
#ifdef WITH_ZEROMQ
  expose<contra::ZMQTransport>();
#endif  // WITH_ZEROMQ
  def("AnyNode", &AnyNode);
  class_<conduit::Node>("Node").def("Update", &conduit::Node::update);
}
SUPPRESS_WARNINGS_END

}  // namespace pycontra
