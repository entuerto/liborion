/*
 * uuid_win32.cpp
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


#include <orion/uuid.h>

#include <cstdint>
#include <rpc.h>

#include <orion/StringUtils.h>

namespace orion
{
//--------------------------------------------------------------------------
// Win32 class declaration

struct Uuid::Private
{
   uuid_t _uuid;
};

Uuid::Uuid() :
   _impl(new Private)
{
   RPC_STATUS ret = UuidCreate(&(_impl->_uuid));

   if (ret != RPC_S_OK)
      UuidCreateNil(&(_impl->_uuid));
}

Uuid::Uuid(const Uuid& rhs) :
   _impl(new Private)
{
   _impl->_uuid = rhs._impl->_uuid;
}

Uuid::Uuid(Uuid&& rhs) :
   _impl(std::move(rhs._impl))
{
}

Uuid::Uuid(const std::string& value) :
   _impl(new Private)
{
   std::wstring str_uuid = utf8_to_wstring(value);

   RPC_STATUS ret = UuidFromStringW((RPC_WSTR)str_uuid.data(), &(_impl->_uuid));

   if (ret != RPC_S_OK)
      UuidCreateNil(&(_impl->_uuid));
}

Uuid::~Uuid()
{
}

bool Uuid::is_null() const
{
   RPC_STATUS status;
   int ret = UuidIsNil(&(_impl->_uuid), &status);

   if (status != RPC_S_OK)
      return false;

   return ret == TRUE;
}

std::string Uuid::to_string() const
{
   RPC_WSTR str_uuid;

   if (UuidToStringW(&(_impl->_uuid), &str_uuid) != RPC_S_OK)
      return "";

   return wstring_to_utf8((wchar_t*)str_uuid);
}

Uuid& Uuid::operator=(const Uuid& rhs)
{
   if (this == &rhs)
      return *this;

   _impl->_uuid = rhs._impl->_uuid;

   return *this;
}

Uuid& Uuid::operator=(Uuid&& rhs)
{
   _impl = std::move(rhs._impl);

   return *this;
}

bool Uuid::operator==(const Uuid& rhs) const
{
   RPC_STATUS status;
   int ret = UuidEqual(&(_impl->_uuid),&(rhs._impl->_uuid), &status);

   if (status != RPC_S_OK)
      return false;

   return ret == TRUE;
}

bool Uuid::operator!=(const Uuid& rhs) const
{
   return not operator==(rhs);
}

}
