//
// AddressV4-win32.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/AddressV4.h>

#include <orion/StringUtils.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>

namespace orion
{
namespace net
{

uint32_t AddressV4::to_ulong() const
{
   return ntohl(_a.s_addr);
}

std::string AddressV4::to_string() const
{
   wchar_t buffer[256];

   RtlIpv4AddressToStringW(&_a, buffer); 

   return wstring_to_utf8(buffer);
}

AddressV4 AddressV4::parse(const std::string& s)
{
   AddressV4 addr;
   uint16_t port = 0;

   auto str = utf8_to_wstring(s);

   RtlIpv4StringToAddressExW(str.data(), TRUE, &addr._a, &port);
   
   auto ec = std::error_code(::GetLastError(), std::system_category());
   if (ec)
      throw std::system_error(ec);

   return addr;
}

} // net
} // orion
