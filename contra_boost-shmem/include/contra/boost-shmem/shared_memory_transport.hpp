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

#ifndef CONTRA_BOOST_SHMEM_INCLUDE_CONTRA_BOOST_SHMEM_SHARED_MEMORY_TRANSPORT_HPP_
#define CONTRA_BOOST_SHMEM_INCLUDE_CONTRA_BOOST_SHMEM_SHARED_MEMORY_TRANSPORT_HPP_

#include <string>
#include <vector>

#include "contra/packet.hpp"
#include "contra/boost-shmem/suppress_warnings.hpp"

#ifdef _WIN32
#define BOOST_INTERPROCESS_SHARED_DIR_FUNC
#endif

SUPPRESS_WARNINGS_BEGIN
#include "boost/interprocess/allocators/allocator.hpp"
#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/sync/named_mutex.hpp"
#include "boost/interprocess/sync/scoped_lock.hpp"
#ifdef _WIN32
#include "boost/interprocess/managed_windows_shared_memory.hpp"
#endif
SUPPRESS_WARNINGS_END

namespace contra {

class SharedMemoryTransport {
 public:
#ifdef _WIN32
  using ManagedSharedMemory =
      boost::interprocess::managed_windows_shared_memory;
#else
  using ManagedSharedMemory = boost::interprocess::managed_shared_memory;
#endif
  using SegmentManager = ManagedSharedMemory::segment_manager;

  using SchemaAllocator = boost::interprocess::allocator<char, SegmentManager>;
  using DataAllocator = boost::interprocess::allocator<uint8_t, SegmentManager>;
  struct InternalPacket {
    using Schema = std::vector<char, SchemaAllocator>;
    using Data = std::vector<uint8_t, DataAllocator>;
    Schema schema;
    Data data;
  };
  using PacketAllocator =
      boost::interprocess::allocator<InternalPacket, SegmentManager>;
  using PacketStorage = std::vector<InternalPacket, PacketAllocator>;

  using NamedMutex = boost::interprocess::named_mutex;
  using ScopedLock = boost::interprocess::scoped_lock<NamedMutex>;

  static constexpr const char* const kDefaultName = "contraShMemTransp";
  static constexpr const char* const kMutexSuffix = "Mtx";

  explicit SharedMemoryTransport(const std::string& name = kDefaultName);
  SharedMemoryTransport(const SharedMemoryTransport&) = delete;
  SharedMemoryTransport(SharedMemoryTransport&&) = delete;
  ~SharedMemoryTransport();

  static void Destroy(const std::string& name = kDefaultName);

  std::size_t GetFreeSize() const;

  void Send(const Packet& packet);
  std::vector<Packet> Receive();

  int GetReferenceCount() const;

  static constexpr const char* SegmentName() { return "packet-shared-memory"; }
  static constexpr const char* MutexName() { return "shared-mutex"; }

  static constexpr const char* PacketStorageName() { return "PacketStorage"; }
  static constexpr const char* ReferenceCountName() { return "ReferenceCount"; }
  static constexpr std::size_t InitialSize() { return 1073741824u; }

  SharedMemoryTransport& operator=(const SharedMemoryTransport&) = delete;
  SharedMemoryTransport& operator=(SharedMemoryTransport&&) = delete;

 private:
  PacketStorage* FindOrConstructPacketStorage();
  int* FindOrConstructReferenceCount();

  ManagedSharedMemory segment_;
  NamedMutex mutex_;

  PacketStorage* packet_storage_;
  int* reference_count_;

  std::string name_;
};

}  // namespace contra

#endif  // CONTRA_BOOST_SHMEM_INCLUDE_CONTRA_BOOST_SHMEM_SHARED_MEMORY_TRANSPORT_HPP_
