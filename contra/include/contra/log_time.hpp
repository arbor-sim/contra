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

#ifndef CONTRA_INCLUDE_CONTRA_LOG_TIME_HPP_
#define CONTRA_INCLUDE_CONTRA_LOG_TIME_HPP_

#include <chrono>
#include <iostream>

#define CONTRA_LOG_HLINE std::cout << std::string(80, '-') << std::endl;
#define CONTRA_LOG_NOTE(note) std::cout << note << std::endl;

#define CONTRA_LOG_HLINE_NOTE(note) \
  CONTRA_LOG_HLINE;                 \
  CONTRA_LOG_NOTE(note)

#define CONTRA_LOG_NOTE_HLINE(note) \
  CONTRA_LOG_NOTE(note);            \
  CONTRA_LOG_HLINE

#define CONTRA_LOG_TIME_BEGIN(note)                              \
  CONTRA_LOG_NOTE(note);                                         \
  auto contra_log_time_start = std::chrono::system_clock::now(); \
  std::chrono::duration<double> contra_log_time_diff

#define CONTRA_LOG_TIME(note)                                   \
  contra_log_time_diff =                                        \
      std::chrono::system_clock::now() - contra_log_time_start; \
  std::cout << note << ": " << contra_log_time_diff.count() << "s" << std::endl

#endif  // CONTRA_INCLUDE_CONTRA_LOG_TIME_HPP_
