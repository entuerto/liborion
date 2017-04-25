/*
 * Uuid.cpp
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


#include <orion/Uuid.h>

#include <uuid/uuid.h>

namespace orion
{
//--------------------------------------------------------------------------
// Win32 class declaration

struct Uuid::Private
{
   uuid_t _uuid;

   Private() : _uuid()
   {}

   ~Private()
   {
   }
};

Uuid::Uuid() :
   _impl(new Private)
{
   uuid_generate(_impl->_uuid);
}

Uuid::Uuid(const Uuid& rhs) :
   _impl(new Private)
{
   uuid_copy(_impl->_uuid, rhs._impl->_uuid);
}

Uuid::Uuid(Uuid&& rhs) :
   _impl(std::move(rhs._impl))
{
}

Uuid::Uuid(const std::string& value) :
   _impl(new Private)
{
   uuid_parse(value.c_str(), _impl->_uuid);
}

Uuid::~Uuid()
{
}

bool Uuid::is_null() const
{
   return uuid_is_null(_impl->_uuid);
}

std::string Uuid::to_string() const
{
   char str_uuid[40];

   uuid_unparse(_impl->_uuid, str_uuid);

   return std::string(str_uuid);
}

Uuid& Uuid::operator=(const Uuid& rhs)
{
   if (this == &rhs)
      return *this;

   uuid_copy(_impl->_uuid, rhs._impl->_uuid);

   return *this;
}

Uuid& Uuid::operator=(Uuid&& rhs)
{
   _impl = std::move(rhs._impl);

   return *this;
}

bool Uuid::operator==(const Uuid& rhs) const
{
   return uuid_compare(_impl->_uuid, rhs._impl->_uuid) == 0;
}

bool Uuid::operator!=(const Uuid& rhs) const
{
   return not operator==(rhs);
}

}
