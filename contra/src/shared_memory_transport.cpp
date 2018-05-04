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
        Allocator(segment_.get_segment_manager()));
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
  packet_storage_->push_back(packet);
}

std::vector<Packet> SharedMemoryTransport::Receive() {
  ScopedLock lock(mutex_);
  std::vector<Packet> received_packets{packet_storage_->begin(),
                                       packet_storage_->end()};
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
