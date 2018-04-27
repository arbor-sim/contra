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

#include "contra/shared_memory_synchronization.hpp"

SUPPRESS_WARNINGS_BEGIN
#include "boost/interprocess/sync/named_mutex.hpp"
#include "boost/interprocess/sync/scoped_lock.hpp"
SUPPRESS_WARNINGS_END

namespace contra {

SharedMemorySynchronization::SharedMemorySynchronization(
    const SharedMemorySynchronization::Create&)
    : mutex_{boost::interprocess::create_only, MutexName()} {}

SharedMemorySynchronization::SharedMemorySynchronization(
    const SharedMemorySynchronization::Access&)
    : mutex_{boost::interprocess::open_only, MutexName()} {}

void SharedMemorySynchronization::Destroy() {
  ManagedMutex::remove(MutexName());
}

SharedMemorySynchronization::ManagedScopedLock
SharedMemorySynchronization::ScopedLock() {
  return ManagedScopedLock(mutex_);
}

bool SharedMemorySynchronization::TryLock() { return mutex_.try_lock(); }

void SharedMemorySynchronization::Unlock() { mutex_.unlock(); }

}  // namespace contra
