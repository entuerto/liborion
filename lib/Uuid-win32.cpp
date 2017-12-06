//
// Uuid-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Uuid.h>

#include <orion/String.h>

#include <cstdint>
#include <rpc.h>

namespace orion
{
//--------------------------------------------------------------------------------------------------
// Win32 class declaration

struct Uuid::Private
{
   uuid_t _uuid;
};

Uuid::Uuid()
   : _impl(new Private)
{
   RPC_STATUS ret = UuidCreate(&(_impl->_uuid));

   if (ret != RPC_S_OK)
   {
      UuidCreateNil(&(_impl->_uuid));
   }
}

Uuid::Uuid(const Uuid& rhs)
   : _impl(new Private)
{
   _impl->_uuid = rhs._impl->_uuid;
}

Uuid::Uuid(Uuid&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

Uuid::Uuid(const std::string& value)
   : _impl(new Private)
{
   std::wstring str_uuid = utf8_to_wstring(value);

   RPC_STATUS ret = UuidFromStringW((RPC_WSTR)str_uuid.data(), &(_impl->_uuid));

   if (ret != RPC_S_OK)
   {
      UuidCreateNil(&(_impl->_uuid));
   }
}

Uuid::~Uuid() = default;

bool Uuid::is_null() const
{
   RPC_STATUS status;
   int ret = UuidIsNil(&(_impl->_uuid), &status);

   if (status != RPC_S_OK)
   {
      return false;
   }

   return ret == TRUE;
}

Uuid& Uuid::operator=(const Uuid& rhs)
{
   if (this == &rhs)
   {
      return *this;
   }

   _impl->_uuid = rhs._impl->_uuid;

   return *this;
}

Uuid& Uuid::operator=(Uuid&& rhs) noexcept
{
   _impl = std::move(rhs._impl);

   return *this;
}

bool Uuid::operator==(const Uuid& rhs) const
{
   RPC_STATUS status;
   int ret = UuidEqual(&(_impl->_uuid), &(rhs._impl->_uuid), &status);

   if (status != RPC_S_OK)
   {
      return false;
   }

   return ret == TRUE;
}

bool Uuid::operator!=(const Uuid& rhs) const
{
   return not operator==(rhs);
}

std::string to_string(const Uuid& id)
{
   RPC_WSTR str_uuid;

   if (UuidToStringW(&(id._impl->_uuid), &str_uuid) != RPC_S_OK)
   {
      return "";
   }

   return wstring_to_utf8((wchar_t*)str_uuid);
}

} // namespace orion
