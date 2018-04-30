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

SharedMemoryTransport::SharedMemoryTransport(const Create&)
    : segment_{boost::interprocess::create_only, SegmentName(), InitialSize()},
      mutex_{boost::interprocess::open_or_create, MutexName()},
      packet_storage_{ConstructPacketStorage()} {}

SharedMemoryTransport::PacketStorage*
SharedMemoryTransport::ConstructPacketStorage() {
  return segment_.construct<PacketStorage>(PacketStorageName())(
      Allocator(segment_.get_segment_manager()));
}

SharedMemoryTransport::SharedMemoryTransport(const Access&)
    : segment_{boost::interprocess::open_only, SegmentName()},
      mutex_{boost::interprocess::open_or_create, MutexName()},
      packet_storage_{FindPacketStorage()} {}

SharedMemoryTransport::PacketStorage*
SharedMemoryTransport::FindPacketStorage() {
  return segment_.find<PacketStorage>(PacketStorageName()).first;
}

std::size_t SharedMemoryTransport::GetFreeSize() const {
  return segment_.get_free_memory();
}

void SharedMemoryTransport::Send(const Packet& packet) {
  // ManagedScopedLock lock(mutex_);
  packet_storage_->push_back(packet);
}

std::vector<Packet> SharedMemoryTransport::Receive() {
  // ManagedScopedLock lock(mutex_);
  std::vector<Packet> received_packets{packet_storage_->begin(),
                                       packet_storage_->end()};
  packet_storage_->clear();
  return received_packets;
}

void SharedMemoryTransport::Destroy() {
  boost::interprocess::shared_memory_object::remove(SegmentName());
  boost::interprocess::named_mutex::remove(MutexName());
}

}  // namespace contra
