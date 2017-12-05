//
// Object.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_OBJECT_H
#define ORION_OBJECT_H

#include <orion/Orion-Stddefs.h>
#include <orion/Uuid.h>
#include <string>

namespace orion
{
/// 
/// Base object
///
class API_EXPORT Object
{
public:
   Object() = default;
   Object(const Object& rhs) = default;
   Object(Object&& rhs) noexcept;
   virtual ~Object() = default;

   virtual Uuid id() const;
   virtual void id(const Uuid& value);

   Object& operator=(const Object& rhs) = default;
   Object& operator=(Object&& rhs) noexcept;

private:
   Uuid _uuid;
};

API_EXPORT std::string to_string(const Object& ob);

} // namespace orion
#endif
