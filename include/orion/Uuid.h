//
// Uuid.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef UUID_H
#define UUID_H

#include <orion/Orion-Stddefs.h>
#include <memory>
#include <string>

namespace orion
{
//! Universally Unique Identifier (uuid).
/*! An uuid provides a unique designation for objects such as interfaces
 *  and client objects.
 *
 *  Implementation note:
 *    On win32 platform the RPC API is used to create uuids.
 *    On linux the libuuid is used.
 */
class API_EXPORT Uuid
{
public:
   Uuid();
   Uuid(const Uuid& rhs);
   Uuid(Uuid&& rhs);
   Uuid(const std::string& value);
   ~Uuid();

   bool is_null() const;

   std::string to_string() const;

   Uuid& operator=(const Uuid& rhs);
   Uuid& operator=(Uuid&& rhs);

   bool operator==(const Uuid& rhs) const;
   bool operator!=(const Uuid& rhs) const;

private:
   struct Private;
   std::unique_ptr<Private> _impl;
};
}
#endif /* UUID_H */
