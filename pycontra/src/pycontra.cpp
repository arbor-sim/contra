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

#include "pycontra.hpp"

#include "contra/contra.hpp"
#include "contra/file_transport.hpp"
#include "contra/relay.hpp"
#include "contra/shared_memory_transport.hpp"
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
extern template void expose<contra::Relay<contra::SharedMemoryTransport>>();

SUPPRESS_WARNINGS_BEGIN
// cppcheck-suppress unusedFunction
BOOST_PYTHON_MODULE(_pycontra) {
  def("Greet", contra::Greet);
  expose<contra::Relay<contra::FileTransport>>();
  expose<contra::Relay<contra::SharedMemoryTransport>>();
  def("AnyNode", &AnyNode);
  class_<conduit::Node>("Node");
}
SUPPRESS_WARNINGS_END

}  // namespace pycontra
