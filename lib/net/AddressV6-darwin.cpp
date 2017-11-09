//
// AddressV6-darwin.cpp
// 
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV6.h>

#include <orion/StringUtils.h>
#include <orion/net/Types.h>

namespace orion
{
namespace net
{

std::string to_string(const AddressV6& addr)
{ 
   char buffer[256];
   
   return inet_ntop(AF_INET6, &addr._a.s6_addr, buffer, 256);
}

AddressV6 make_address_v6(const std::string& s)
{
   std::array<uint8_t, 16> bytes;
   
   if (inet_pton(AF_INET6, s.c_str(), &bytes) < 0)
   {
      auto ec = std::error_code(errno, std::system_category());
      throw std::system_error(ec);
   }
   return AddressV6(bytes, 0);
}

} // net
} // orion
