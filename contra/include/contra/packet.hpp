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

#ifndef CONTRA_INCLUDE_CONTRA_PACKET_HPP_
#define CONTRA_INCLUDE_CONTRA_PACKET_HPP_

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace contra {

template <typename SchemaAllocator = std::allocator<char>,
          typename DataAllocator = std::allocator<uint8_t>>
struct BasicPacket {
  using SchemaString =
      std::basic_string<char, std::char_traits<char>, SchemaAllocator>;
  using DataVector = std::vector<uint8_t, DataAllocator>;

  template <typename T = int,
            typename = std::enable_if_t<
                std::is_default_constructible<SchemaAllocator>::value &&
                    std::is_default_constructible<DataAllocator>::value,
                T>>
  BasicPacket() {}  // = default does not compile on MSVC

  template <typename T = int, typename OtherSchemaAllocator,
            typename OtherDataAllocator,
            typename = std::enable_if_t<
                std::is_default_constructible<SchemaAllocator>::value &&
                    std::is_default_constructible<DataAllocator>::value,
                T>>
  BasicPacket(
      const BasicPacket<OtherSchemaAllocator, OtherDataAllocator>& other)
      : schema(other.schema.begin(), other.schema.end()),
        data(other.data.begin(), other.data.end()) {}

  BasicPacket(const SchemaAllocator& schema_allocator,
              const DataAllocator& data_allocator)
      : schema(schema_allocator), data(data_allocator) {}

  template <typename SchemaIterator, typename DataIterator>
  BasicPacket(SchemaIterator&& schema_begin, SchemaIterator&& schema_end,
              const SchemaAllocator& schema_allocator,
              DataIterator&& data_begin, DataIterator&& data_end,
              const DataAllocator& data_allocator)
      : schema(std::forward<SchemaIterator>(schema_begin),
               std::forward<SchemaIterator>(schema_end), schema_allocator),
        data(std::forward<DataIterator>(data_begin),
             std::forward<DataIterator>(data_end), data_allocator) {}

  template <typename T = int,
            typename = std::enable_if_t<
                std::is_default_constructible<SchemaAllocator>::value &&
                    std::is_default_constructible<DataAllocator>::value,
                T>>
  BasicPacket(const SchemaString& schema, const DataVector& data)
      : schema{std::forward<const SchemaString&>(schema)},
        data{std::forward<const DataVector&>(data)} {}

  SchemaString schema;
  DataVector data;
};

using Packet = BasicPacket<>;

}  // namespace contra

#endif  // CONTRA_INCLUDE_CONTRA_PACKET_HPP_
