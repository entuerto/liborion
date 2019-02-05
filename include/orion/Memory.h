//
// Memory.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_MEMORY_UTILS_H
#define ORION_MEMORY_UTILS_H

#include <orion/Config.h>

#include <memory>

namespace orion
{
template<typename R, typename T>
API_EXPORT inline std::shared_ptr<R> iface_cast(const std::shared_ptr<T>& other)
{
   return std::dynamic_pointer_cast<R>(other);
}

template<typename R, typename T>
inline R iface_cast(T other)
{
   auto ptr = dynamic_cast<R>(other);

   if (ptr == nullptr)
      return static_cast<R>(*other);

   return ptr;
}
}
#endif
