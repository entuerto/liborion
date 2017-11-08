//
// AddrressV4-darwin.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV4.h>

#include <orion/StringUtils.h>

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
   
   return "";
}

AddressV4 make_address_v4(const std::string& value)
{
   
   return AddressV4();
}

} // net
} // orion
