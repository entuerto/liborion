//
// Uuid-darwin.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Uuid.h>

#include <uuid/uuid.h>

namespace orion
{
//--------------------------------------------------------------------------
// Darwin class declaration

struct Uuid::Private
{
   uuid_t _uuid;
};

Uuid::Uuid()
   : _impl(new Private)
{
   uuid_generate(_impl->_uuid);
}

Uuid::Uuid(const Uuid& rhs)
   : _impl(new Private)
{
   uuid_copy(_impl->_uuid, rhs._impl->_uuid);
}

Uuid::Uuid(Uuid&& rhs) noexcept
   : _impl(std::move(rhs._impl))
{
}

Uuid::Uuid(const std::string& value)
   : _impl(new Private)
{
   uuid_parse(value.c_str(), _impl->_uuid);
}

Uuid::~Uuid() = default;

bool Uuid::is_null() const
{
   return uuid_is_null(_impl->_uuid);
}

Uuid& Uuid::operator=(const Uuid& rhs)
{
   if (this == &rhs)
      return *this;

   uuid_copy(_impl->_uuid, rhs._impl->_uuid);

   return *this;
}

Uuid& Uuid::operator=(Uuid&& rhs) noexcept
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

std::string to_string(const Uuid& id)
{
   char str_uuid[40];

   uuid_unparse(id._impl->_uuid, str_uuid);

   return std::string(str_uuid);
}

} // namespace orion
