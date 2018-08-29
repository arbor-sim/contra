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

#ifndef PYCONTRA_SRC_PYCONTRA_HPP_
#define PYCONTRA_SRC_PYCONTRA_HPP_

#include "pycontra/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#if __GNUC__ >= 7
#pragma GCC diagnostic ignored "-Wregister"
#endif
#include "boost/python.hpp"
SUPPRESS_WARNINGS_END

using boost::noncopyable;
using boost::python::args;
using boost::python::bases;
using boost::python::class_;
using boost::python::def;
using boost::python::enum_;
using boost::python::init;
using boost::python::no_init;
using boost::python::pure_virtual;
using boost::python::scope;
using boost::python::wrapper;

namespace pycontra {

template <typename T>
void expose();

}  // namespace pycontra

#endif  // PYCONTRA_SRC_PYCONTRA_HPP_
