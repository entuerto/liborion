//
// Uuid.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef UUID_H
#define UUID_H

#include <orion/Config.h>

#include <memory>
#include <string>

namespace orion
{
/// Universally Unique Identifier (uuid).
/// An uuid provides a unique designation for objects such as interfaces
/// and client objects.
///
/// Implementation note:
///   On win32 platform the RPC API is used to create uuids.
///   On linux the libuuid is used.
///
class API_EXPORT Uuid
{
public:
   Uuid();
   Uuid(const Uuid& rhs);
   Uuid(Uuid&& rhs) noexcept;
   explicit Uuid(const std::string& value);
   ~Uuid();

   bool is_null() const;

   Uuid& operator=(const Uuid& rhs);
   Uuid& operator=(Uuid&& rhs) noexcept;

   bool operator==(const Uuid& rhs) const;
   bool operator!=(const Uuid& rhs) const;

   friend API_EXPORT std::string to_string(const Uuid& id);

private:
   struct Private;
   std::unique_ptr<Private> _impl;
};

API_EXPORT std::string to_string(const Uuid& id);

}
#endif /* UUID_H */
