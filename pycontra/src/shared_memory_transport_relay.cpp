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

#include <string>

#include "contra/relay.hpp"
#include "contra/shared_memory_transport.hpp"
#include "contra/suppress_warnings.hpp"
#include "pycontra.hpp"

namespace pycontra {

SUPPRESS_WARNINGS_BEGIN
boost::python::list SharedMemoryTransportRelayReceive(
    contra::Relay<contra::SharedMemoryTransport>* relay) {
  boost::python::list ret_val;
  for (const auto& node : relay->Receive()) {
    ret_val.append(node);
  }
  return ret_val;
}
SUPPRESS_WARNINGS_END

template <>
void expose<contra::Relay<contra::SharedMemoryTransport>>() {
  class_<contra::Relay<contra::SharedMemoryTransport>, boost::noncopyable>(
      "SharedMemoryTransportRelay")
      .def("Send", &contra::Relay<contra::SharedMemoryTransport>::Send)
      .def("Receive", &SharedMemoryTransportRelayReceive);
}

}  // namespace pycontra
