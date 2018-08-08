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

#include <utility>

#include "catch/catch.hpp"

#include "contra/file_transport.hpp"
#include "contra/relay.hpp"
#include "contra/zmq/zeromq_transport.hpp"

#include "contra/test_utilities/conduit_data.hpp"
#include "contra/test_utilities/conduit_node_matcher.hpp"

template <typename Class, typename Tuple, std::size_t... Inds>
Class help_make_SomeClass(Tuple&& tuple, std::index_sequence<Inds...>) {
  return Class(std::get<Inds>(std::forward<Tuple>(tuple))...);
}

template <typename Class, typename Tuple>
Class make_SomeClass(Tuple&& tuple) {
  return help_make_SomeClass<Class>(
      std::forward<Tuple>(tuple),
      std::make_index_sequence<std::tuple_size<Tuple>::value>());
}

#define RELAY_TRANSPORT_TEST(transport_type, sender_params, receiver_params) \
  contra::Relay<transport_type> sender =                                     \
      make_SomeClass<contra::Relay<transport_type>>(sender_params);          \
  contra::Relay<transport_type> receiver =                                   \
      make_SomeClass<contra::Relay<transport_type>>(receiver_params);        \
                                                                             \
  sender.Send(test_utilities::ANY_NODE);                                     \
  const auto received_nodes = receiver.Receive();                            \
                                                                             \
  REQUIRE(received_nodes.size() == 1);                                       \
  REQUIRE_THAT(received_nodes[0], Equals(test_utilities::ANY_NODE));

SCENARIO("Data gets transported via ZMQTransport", "[contra][contra::Relay]") {
  RELAY_TRANSPORT_TEST(
      contra::ZMQTransport,
      std::make_tuple(contra::ZMQTransport::Type::SERVER, "tcp://*:5555", true),
      std::make_tuple(contra::ZMQTransport::Type::CLIENT,
                      "tcp://localhost:5555", true));
}
