// Utils.h
//
// Copyright 2013 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef ORION_MEMORY_UTILS_H
#define ORION_MEMORY_UTILS_H

#include <memory>

#define DECLARE_POINTERS(P) \
   typedef std::unique_ptr<P> UniquePtr; \
   typedef std::shared_ptr<P> SharedPtr; \
   typedef std::weak_ptr<P> WeakPtr; 

namespace orion
{
template<typename R, typename T>
inline std::shared_ptr<R> iface_cast(const std::shared_ptr<T>& other)
{
   return std::dynamic_pointer_cast<R>(other);
}

template<typename R, typename T>
inline R* iface_cast(const T& other)
{
   return dynamic_cast<R*>(other);
}

}
#endif

