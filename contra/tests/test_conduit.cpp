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

#include <iterator>
#include <string>
#include <vector>

#include "catch2/catch.hpp"

#include "conduit/conduit_node.hpp"

#include "contra/test_utilities/conduit_data.hpp"
#include "contra/test_utilities/conduit_node_matcher.hpp"
#include "contra/test_utilities/conduit_schema.hpp"

namespace conduit {

template <>
DataArray<uint64>::~DataArray();

}  // namespace conduit

SCENARIO("conduit array leafs are compatible to std::vector", "[conduit]") {
  const std::string some_path{"root/inner/leaf"};

  GIVEN("a std vector assigning data to a node") {
    const std::vector<std::size_t> some_data{1, 5, 7, 9};
    conduit::Node node;
    node[some_path].set(some_data);

    THEN("data is stored in the node") {
      for (std::size_t i = 0; i < some_data.size(); ++i) {
        REQUIRE(node[some_path]
                    .as_uint64_array()[static_cast<conduit::index_t>(i)] ==
                some_data[i]);
      }
    }

    WHEN("data is retrieved from the node into a vector") {
      const auto& node_data = node[some_path].as_uint64_array();
      const conduit::index_t num_elements = node_data.number_of_elements();
      const auto* begin = reinterpret_cast<std::size_t*>(node_data.data_ptr());
      const auto* end = begin + num_elements;
      std::vector<std::size_t> retrieved_data(begin, end);

      THEN("the vector and the original data are the same") {
        REQUIRE(retrieved_data == some_data);
      }
    }
  }
}

namespace {

void SerializeConstRef(const conduit::Node& node, std::string* schema,
                       std::vector<conduit::uint8>* bytes) {
  conduit::Schema compact_schema;
  node.schema().compact_to(compact_schema);
  const std::string compact_schema_json(compact_schema.to_json());
  schema->assign(compact_schema_json.begin(), compact_schema_json.end());

  // intermediate vector not required in general, but our use case requires it
  std::vector<conduit::uint8> data;
  node.serialize(data);
  bytes->assign(data.begin(), data.end());
}

}  // namespace

SCENARIO(
    "multiple serialization and re-read works. Conduit's issues #226, #229 are "
    "fixed."
    "[conduit]") {
  INFO(
      "This test's failing indicates that something might have broken the "
      "fixes for conduit's issues #226, #229.\n"
      "See \n"
      "* https://github.com/LLNL/conduit/issues/226 \n"
      "* https://github.com/LLNL/conduit/issues/229 \n")
  GIVEN("a node that is serialized and read back") {
    std::string schema;
    std::vector<conduit::uint8> bytes;

    SerializeConstRef(test_utilities::ANY_NODE, &schema, &bytes);

    conduit::Node second_node;
    second_node.set_data_using_schema(conduit::Schema(schema), bytes.data());

    WHEN("the second node is serialized and read into a third node") {
      SerializeConstRef(second_node, &schema, &bytes);

      conduit::Node third_node;
      third_node.set_data_using_schema(conduit::Schema(schema), bytes.data());

      REQUIRE(third_node.to_json() == test_utilities::ANY_NODE.to_json());
    }
  }
}

SCENARIO(
    "node changes are not yet reflected by nodes using external data "
    "(conduit's #240)",
    "[conduit]") {
  INFO(
      "This test's failing indicates that conduit's #240 might be addressed. \n"
      "On failure: \n"
      "* Nodes referring to others via external might reflect changes. \n"
      "* Update niv's code to utilize this")
  GIVEN("a node and one referring to it") {
    conduit::Node original_node;
    conduit::Node referring_node;

    WHEN("the refering node uses the original one as external data") {
      referring_node.set_external(original_node);

      WHEN("the original node is changed") {
        original_node["A"] = 3.1415;

        THEN("the change is visible in the referring node") {
          REQUIRE_THAT(referring_node, !Equals(original_node));
          REQUIRE(referring_node.dtype().is_empty());
        }
      }

      WHEN("the referring node is changed") {
        referring_node["B"] = 4.123;

        THEN("the change is visible in the original node") {
          REQUIRE_THAT(original_node, !Equals(referring_node));
          REQUIRE(original_node.dtype().is_empty());
        }
      }

      WHEN("the original node is updated") {
        conduit::Node update;
        update["A"] = 3.1415;
        original_node.update(update);

        THEN("the change is visible in the referring node") {
          REQUIRE_THAT(referring_node, !Equals(original_node));
          REQUIRE(referring_node.dtype().is_empty());
        }
      }

      WHEN("the referring node is updated") {
        conduit::Node update;
        update["B"] = 4.123;
        referring_node.update(update);

        THEN("the change is visible in the original node") {
          REQUIRE_THAT(original_node, !Equals(referring_node));
          REQUIRE(original_node.dtype().is_empty());
        }
      }
    }
  }
}

SCENARIO("update inserts new nodes", "[conduit]") {
  GIVEN("A conduit tree") {
    conduit::Node a = test_utilities::ANY_NODE;

    WHEN("A second node updates the tree") {
      conduit::Node b = test_utilities::ANY_UPDATE;
      a.update(b);

      THEN("the first node contains also the content of the second") {
        REQUIRE_THAT(a, Equals(test_utilities::UPDATED_NODE));
      }
    }
  }
}

SCENARIO("node updates into empty node with unexpected order", "[conduit]") {
  GIVEN("an empty conduit node") {
    conduit::Node target;
    WHEN("the target node is updated with some data in unexpected order") {
      target.update(test_utilities::ANY_UPDATE);
      target.update(test_utilities::ANY_NODE);
      THEN("the node's layout is unexpected") {
        REQUIRE_THAT(target, !Equals(test_utilities::UPDATED_NODE));
      }
    }
  }
}

SCENARIO("node updates into pre-allocated node with unexpected order",
         "[conduit]") {
  GIVEN("an allocated conduit node") {
    conduit::Node preallocated{test_utilities::UPDATED_NODE_ALL_ZEROS};
    WHEN("the node is updated with some data in unexpected order") {
      preallocated.update(test_utilities::ANY_UPDATE);
      preallocated.update(test_utilities::ANY_NODE);
      THEN("the node's layout is as expected") {
        REQUIRE_THAT(preallocated, Equals(test_utilities::UPDATED_NODE));
      }
    }
  }
}

SCENARIO("conduit data layout", "[conduit]") {
  GIVEN("a compacted conduit node") {
    conduit::Node node;
    test_utilities::UPDATED_NODE.compact_to(node);

    THEN("the node's data is contiguous") { REQUIRE(node.is_contiguous()); }
    WHEN("the node's data is accessed via ptr") {
      const double* data_ptr =
          reinterpret_cast<const double*>(node.contiguous_data_ptr());

      THEN("the leafs' data is accessible as an array") {
        REQUIRE(data_ptr[0] ==
                test_utilities::UPDATED_NODE["A/B/C"].as_double());
        REQUIRE(data_ptr[1] ==
                test_utilities::UPDATED_NODE["A/B/D"].as_double());
        REQUIRE(data_ptr[2] ==
                test_utilities::UPDATED_NODE["A/B/F"].as_double());
        REQUIRE(data_ptr[3] ==
                test_utilities::UPDATED_NODE["A/B/G"].as_double());
        REQUIRE(data_ptr[4] == test_utilities::UPDATED_NODE["A/E"].as_double());
        REQUIRE(data_ptr[5] == test_utilities::UPDATED_NODE["A/H"].as_double());
      }
    }
  }
}

SCENARIO("create conduit::Node from data and schema (string)", "[conduit]") {
  GIVEN("a schema and data") {
    const std::string schema{
        "{ \n"
        "  \"A\":{ \n"
        "    \"B\":{\"dtype\":\"float64\", \"number_of_elements\": 1, \n"
        "           \"offset\": 0, \"stride\": 8, \"element_bytes\": 8}, \n"
        "    \"C\":{\"dtype\":\"float64\", \"number_of_elements\": 1, \n"
        "           \"offset\": 8, \"stride\": 8, \"element_bytes\": 8} \n"
        "  }, \n"
        "  \"D\":{\"dtype\":\"float64\", \"number_of_elements\": 1, \n"
        "         \"offset\": 16, \"stride\": 8, \"element_bytes\": 8} \n"
        "}"};
    std::vector<double> data{1.23, 2.34, 3.45};

    WHEN("a node is created from that") {
      conduit::Node node(schema, reinterpret_cast<void*>(data.data()), true);

      THEN("it contains the expected data") {
        REQUIRE(node["A/B"].as_double() == data[0]);
        REQUIRE(node["A/C"].as_double() == data[1]);
        REQUIRE(node["D"].as_double() == data[2]);
      }
    }
  }
}

namespace {

const char* ANY_TAG{"A"};

}  // namespace

SCENARIO("create conduit::Node from data and schema (stringstream)",
         "[conduit]") {
  GIVEN("a schema and data") {
    std::stringstream schema;
    schema << "{\n";
    schema << "  " << test_utilities::conduit_schema::OpenTag(::ANY_TAG);
    schema << "    " << test_utilities::conduit_schema::OpenTag("B");
    schema << "      " << test_utilities::conduit_schema::DoubleData(0);
    schema << "    " << test_utilities::conduit_schema::CloseTagNext();
    schema << "    " << test_utilities::conduit_schema::OpenTag("C");
    schema << "      " << test_utilities::conduit_schema::DoubleData(8);
    schema << "    " << test_utilities::conduit_schema::CloseTag();
    schema << "  " << test_utilities::conduit_schema::CloseTagNext();
    schema << "  " << test_utilities::conduit_schema::OpenTag("D");
    schema << "    " << test_utilities::conduit_schema::DoubleData(16);
    schema << "  " << test_utilities::conduit_schema::CloseTag();
    schema << "}";

    std::vector<double> data{1.23, 2.34, 3.45};

    WHEN("a node is created from that") {
      conduit::Node node(schema.str(), reinterpret_cast<void*>(data.data()),
                         true);

      THEN("it contains the expected data") {
        REQUIRE(node["A/B"].as_double() == data[0]);
        REQUIRE(node["A/C"].as_double() == data[1]);
        REQUIRE(node["D"].as_double() == data[2]);
      }
    }
  }
}

SCENARIO("conduit::Node::getch_child(path) behaves as intended", "[conduit]") {
  GIVEN("a const ptr to a node with some data") {
    conduit::Node node;
    node["A/B/C"] = test_utilities::ANY_VALUE;
    const conduit::Node* const node_ptr{&node};

    WHEN("fetch_child is called with a correct path") {
      THEN("it does not throw and yield the correct datum") {
        const conduit::Node* retrieved_node_ptr{nullptr};
        REQUIRE_NOTHROW(retrieved_node_ptr = &node_ptr->fetch_child("A/B/C"));
        REQUIRE(retrieved_node_ptr->as_double() == test_utilities::ANY_VALUE);
      }
    }

    WHEN("fetch_child is called with an incorrect path; completely off") {
      THEN("it throws") {
        const conduit::Node* retrieved_node_ptr{nullptr};
        REQUIRE_THROWS(retrieved_node_ptr =
                           &node_ptr->fetch_child("FOO/BAR/FOOBAR"));
      }
    }

    WHEN("fetch_child is called with an incorrect path; error at front") {
      THEN("it throws") {
        const conduit::Node* retrieved_node_ptr{nullptr};
        REQUIRE_THROWS(retrieved_node_ptr =
                           &node_ptr->fetch_child("ERROR/B/C"));
      }
    }

    WHEN("fetch_child is called with an incorrect path; error at middle") {
      THEN("it throws") {
        const conduit::Node* retrieved_node_ptr{nullptr};
        REQUIRE_THROWS(retrieved_node_ptr =
                           &node_ptr->fetch_child("A/ERROR/C"));
      }
    }

    WHEN("fetch_child is called with an incorrect path; error at end") {
      THEN("it throws") {
        const conduit::Node* retrieved_node_ptr{nullptr};
        REQUIRE_THROWS(retrieved_node_ptr =
                           &node_ptr->fetch_child("A/B/ERROR"));
      }
    }
  }
}

SCENARIO(
    "Node copy does not preserve externalness as intended by conduit."
    "[conduit]") {
  INFO(
      "This test's failing indicates that the intended behaviour of conduit "
      "has changed. \n"
      "On failure: Copying preserves externalness. \n"
      "* Check back with the conduit developers, if this is intended. \n"
      "* Simplify listening, e.g., in niv::ConduitReceiver::Start(...) "
      "to not use set_external(...) anymore.")
  GIVEN("An external conduit node") {
    std::string schema = test_utilities::ANY_NODE.schema().to_json();
    std::vector<conduit::uint8> data;
    test_utilities::ANY_NODE.serialize(data);

    constexpr bool external{true};
    conduit::Node external_node(schema, data.data(), external);
    const std::string original_json{external_node.to_json()};

    GIVEN("a copy of the external node") {
      conduit::Node copied_node(external_node);
      std::string original_json_copied{copied_node.to_json()};

      THEN("The two nodes are equal") {
        REQUIRE_THAT(external_node, Equals(copied_node));
      }

      WHEN("the data is changed") {
        data[0] = ~data[0];
        data[7] = ~data[7];
        THEN("the external node has changed data") {
          REQUIRE(external_node.to_json() != original_json);
        }
        THEN("the copied node has changed data") {
          REQUIRE_FALSE(copied_node.to_json() != original_json_copied);
        }
        THEN("the copied node is still equal to the external one") {
          REQUIRE_FALSE(external_node.to_json() == copied_node.to_json());
        }
      }
    }

    GIVEN("the external node assigned to a new one") {
      conduit::Node assigned_node;
      assigned_node = external_node;
      std::string original_json_assigned{assigned_node.to_json()};

      THEN("The two nodes are equal") {
        REQUIRE_THAT(external_node, Equals(assigned_node));
      }

      WHEN("the data is changed") {
        data[0] = ~data[0];
        data[7] = ~data[7];
        THEN("the external node has changed data") {
          REQUIRE(external_node.to_json() != original_json);
        }
        THEN("the assigned node has changed data") {
          REQUIRE_FALSE(assigned_node.to_json() != original_json_assigned);
        }
        THEN("the copied node is still equal to the external one") {
          REQUIRE_FALSE(external_node.to_json() == assigned_node.to_json());
        }
      }
    }
  }
}
