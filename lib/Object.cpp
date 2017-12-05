//
// Object.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Object.h>

namespace orion
{

Object::Object(Object&& rhs) noexcept
   : _uuid(std::move(rhs._uuid))
{
}

Uuid Object::id() const
{
   return _uuid;
}

void Object::id(const Uuid& value)
{
   _uuid = value;
}

Object& Object::operator=(Object&& rhs) noexcept
{
   _uuid = std::move(rhs._uuid);

   return *this;
}

std::string to_string(const Object& ob)
{
   return to_string(ob.id());
}

} // namespace orion
