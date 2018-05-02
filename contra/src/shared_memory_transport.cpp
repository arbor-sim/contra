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
#include <chrono>
#include <iostream>
#include <memory>
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
    : segment_{CreateSegment()},
      mutex_{CreateMutex()},
      packet_storage_{ConstructPacketStorage()} {}

std::unique_ptr<SharedMemoryTransport::ManagedSharedMemory>
SharedMemoryTransport::CreateSegment() {
  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> diff;

  auto ret_val = std::make_unique<ManagedSharedMemory>(
      boost::interprocess::create_only, SegmentName(), InitialSize());

  diff = std::chrono::system_clock::now() - start;
  std::cout << "SharedMemoryTransport::CreateSegment(): " << diff.count()
            << "s\n";
  return ret_val;
}

std::unique_ptr<SharedMemoryTransport::ManagedMutex>
SharedMemoryTransport::CreateMutex() {
  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> diff;

  auto ret_val = std::make_unique<ManagedMutex>(
      boost::interprocess::open_or_create, MutexName());

  diff = std::chrono::system_clock::now() - start;
  std::cout << "SharedMemoryTransport::CreateMutex(): " << diff.count()
            << "s\n";
  return ret_val;
}

SharedMemoryTransport::PacketStorage*
SharedMemoryTransport::ConstructPacketStorage() {
  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> diff;

  auto ret_val = segment_->construct<PacketStorage>(PacketStorageName())(
      Allocator(segment_->get_segment_manager()));

  diff = std::chrono::system_clock::now() - start;
  std::cout << "SharedMemoryTransport::ConstructPacketStorage(): "
            << diff.count() << "s\n";

  return ret_val;
}

SharedMemoryTransport::SharedMemoryTransport(const Access&)
    : segment_{AccessSegment()},
      mutex_{AccessMutex()},
      packet_storage_{FindPacketStorage()} {}

std::unique_ptr<SharedMemoryTransport::ManagedSharedMemory>
SharedMemoryTransport::AccessSegment() {
  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> diff;

  auto ret_val = std::make_unique<ManagedSharedMemory>(
      boost::interprocess::open_only, SegmentName());

  diff = std::chrono::system_clock::now() - start;
  std::cout << "SharedMemoryTransport::AccessSegment(): " << diff.count()
            << "s\n";
  return ret_val;
}

std::unique_ptr<SharedMemoryTransport::ManagedMutex>
SharedMemoryTransport::AccessMutex() {
  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> diff;

  auto ret_val = std::make_unique<ManagedMutex>(
      boost::interprocess::open_or_create, MutexName());

  diff = std::chrono::system_clock::now() - start;
  std::cout << "SharedMemoryTransport::AccessMutex(): " << diff.count()
            << "s\n";
  return ret_val;
}

SharedMemoryTransport::PacketStorage*
SharedMemoryTransport::FindPacketStorage() {
  auto start = std::chrono::system_clock::now();
  std::chrono::duration<double> diff;

  auto* packet_storage =
      segment_->find<PacketStorage>(PacketStorageName()).first;

  diff = std::chrono::system_clock::now() - start;
  std::cout << "SharedMemoryTransport::FindPacketStorage(): " << diff.count()
            << std::endl;
  return packet_storage;
}

std::size_t SharedMemoryTransport::GetFreeSize() const {
  return segment_->get_free_memory();
}

void SharedMemoryTransport::Send(const Packet& packet) {
  ManagedScopedLock lock(*mutex_);
  packet_storage_->push_back(packet);
}

std::vector<Packet> SharedMemoryTransport::Receive() {
  ManagedScopedLock lock(*mutex_);
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
