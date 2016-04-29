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

namespace orion
{
//--------------------------------------------------------------------------
// Win32 class declaration

struct Uuid::Private
{
   uuid_t _uuid;
   unsigned char* _str_uuid;

   Private() : _str_uuid(NULL)
   {}

   ~Private()
   {
      if (_str_uuid != NULL)
         RpcStringFree(&_str_uuid);
   }
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

Uuid::Uuid(const std::string& value) :
   _impl(new Private)
{
   uint8_t* str_uuid = (uint8_t*)value.c_str();

   RPC_STATUS ret = UuidFromString(str_uuid, &(_impl->_uuid));

   if (ret != RPC_S_OK)
      UuidCreateNil(&(_impl->_uuid));
}

Uuid::~Uuid()
{
   delete _impl;
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
   if (_impl->_str_uuid == NULL) {
      if (UuidToString(&(_impl->_uuid), &(_impl->_str_uuid)) != RPC_S_OK)
         return "";
   }

   return std::string(reinterpret_cast<char*>(_impl->_str_uuid));
}

Uuid& Uuid::operator=(const Uuid& rhs)
{
   if (this == &rhs)
      return *this;

   _impl->_uuid     = rhs._impl->_uuid;

   if (_impl->_str_uuid != NULL)
      RpcStringFree(&(_impl->_str_uuid));

   _impl->_str_uuid = NULL;

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
