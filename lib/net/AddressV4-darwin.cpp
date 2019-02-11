//
// AddrressV4-darwin.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/Address.h>

#include <orion/String.h>
#include <orion/net/Types.h>

namespace orion
{
namespace net
{

AddressV4::AddressV4(uint32_t value)
   : _a()
{
   _a.s_addr = htonl(value);
}

uint32_t AddressV4::to_ulong() const
{
   return ntohl(_a.s_addr);
}

std::string to_string(const AddressV4& addr)
{
   char buffer[256];

   return inet_ntop(AF_INET, &addr._a.s_addr, buffer, 256);
}

AddressV4 make_address_v4(const std::string& value)
{
   std::array<uint8_t, 4> bytes;

   if (inet_pton(AF_INET, value.c_str(), &bytes) < 0)
   {
      auto ec = std::error_code(errno, std::system_category());
      throw std::system_error(ec);
   }
   return AddressV4(bytes);
}

} // net
} // orion
