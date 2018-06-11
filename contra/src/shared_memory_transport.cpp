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

#include "contra/shared_memory_transport.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
namespace boost {
namespace interprocess {
namespace ipcdetail {
// cppcheck-suppress unusedFunction
inline void get_shared_dir(
    std::string& shared_dir) {  // NOLINT runtime/references
  shared_dir = ".";
}
}  // namespace ipcdetail
}  // namespace interprocess
}  // namespace boost
#endif

namespace contra {

SharedMemoryTransport::SharedMemoryTransport(const std::string& name)
    : segment_{boost::interprocess::open_or_create, name.c_str(),
               InitialSize()},
      mutex_{boost::interprocess::open_or_create,
             (name + kMutexSuffix).c_str()},
      packet_storage_{FindOrConstructPacketStorage()},
      reference_count_{FindOrConstructReferenceCount()},
      name_{name} {
  ++(*reference_count_);
}

SharedMemoryTransport::PacketStorage*
SharedMemoryTransport::FindOrConstructPacketStorage() {
  PacketStorage* storage =
      segment_.find<PacketStorage>(PacketStorageName()).first;
  if (storage == nullptr) {
    storage = segment_.construct<PacketStorage>(PacketStorageName())(
        PacketAllocator(segment_.get_segment_manager()));
  }
  return storage;
}

int* SharedMemoryTransport::FindOrConstructReferenceCount() {
  int* count = segment_.find<int>(ReferenceCountName()).first;
  if (count == nullptr) {
    count = segment_.construct<int>(ReferenceCountName())(0);
  }
  return count;
}

SharedMemoryTransport::~SharedMemoryTransport() {
  --(*reference_count_);
  if (*reference_count_ == 0) {
    Destroy(name_);
  }
}

std::size_t SharedMemoryTransport::GetFreeSize() const {
  return segment_.get_free_memory();
}

void SharedMemoryTransport::Send(const Packet& packet) {
  ScopedLock lock(mutex_);
  std::cout << "Before push: " << packet_storage_->size() << std::endl;
  InternalPacket internal_packet{
      packet.schema.begin(),
      packet.schema.end(),
      SchemaAllocator{segment_.get_segment_manager()},
      packet.data.begin(),
      packet.data.end(),
      DataAllocator{segment_.get_segment_manager()}};

  packet_storage_->push_back(internal_packet);
  std::cout << "Before push: " << packet_storage_->size() << std::endl;
}  // namespace contra

std::vector<Packet> SharedMemoryTransport::Receive() {
  ScopedLock lock(mutex_);
  std::cout << "Before receive: " << packet_storage_->size() << std::endl;
  std::cout << "                " << packet_storage_->data() << std::endl;

  std::vector<Packet> received_packets;
  received_packets.reserve(packet_storage_->size());

  for (const auto& internal_packet : *packet_storage_) {
    // std::cout << "Packet {" << std::endl;
    // std::cout << "    schema: " << internal_packet.schema << std::endl;
    // std::cout << "    data:   [" << internal_packet.data.size() << ""
    //           << std::endl;
    // std::cout << "}" << std::endl;
    received_packets.push_back(internal_packet);
  }
  packet_storage_->clear();
  return received_packets;
}

void SharedMemoryTransport::Destroy(const std::string& name) {
  boost::interprocess::shared_memory_object::remove(name.c_str());
  boost::interprocess::named_mutex::remove((name + kMutexSuffix).c_str());
}

int SharedMemoryTransport::GetReferenceCount() const {
  return *reference_count_;
}

}  // namespace contra
