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
//! Base object
/*!
 */
class API_EXPORT Object
{
public:
   Object();
   Object(const Object& rhs);
   Object(Object&& rhs);
   virtual ~Object();

   virtual Uuid id() const;
   virtual void id(const Uuid& value);

   virtual std::string to_string() const;

   Object& operator=(const Object& rhs);
   Object& operator=(Object&& rhs);

private:
   Uuid _uuid;
};

} // namespace orion
#endif
