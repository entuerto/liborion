/*
 * uuid.h
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef UUID_H
#define UUID_H

#include <memory>
#include <string>
#include <orion/Orion-Stddefs.h>

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
