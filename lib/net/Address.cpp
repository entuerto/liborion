//
// Address.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Address.h>

#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>
#include <orion/net/Error.h>

namespace orion
{
namespace net
{

std::string to_string(const Address& addr)
{
   return to_string(&addr);
}

std::string to_string(const Address* addr)
{
   auto ip_v4 = dynamic_cast<const AddressV4*>(addr);

   if (ip_v4 != nullptr)
      return to_string(*ip_v4);

   auto ip_v6 = dynamic_cast<const AddressV6*>(addr);

   if (ip_v6 != nullptr)
      return to_string(*ip_v6);

   throw std::system_error(make_error_code(ErrorCode::InvalidAddress));
}

} // net
} // orion
