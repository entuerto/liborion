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

Object::Object()
   : _uuid()
{
}

Object::Object(const Object& rhs)
   : _uuid(rhs._uuid)
{
}

Object::Object(Object&& rhs)
   : _uuid(std::move(rhs._uuid))
{
}

Object::~Object()
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

Object& Object::operator=(const Object& rhs)
{
   if (this == &rhs)
      return *this;

   _uuid = rhs._uuid;

   return *this;
}

Object& Object::operator=(Object&& rhs)
{
   _uuid = std::move(rhs._uuid);

   return *this;
}

std::string Object::to_string() const
{
   return _uuid.to_string();
}

} // namespace nodus
