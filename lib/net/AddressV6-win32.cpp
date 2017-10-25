/*
 * IPv6-win32.cpp
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

#include <orion/net/AddressV6.h>

#include <orion/StringUtils.h>

#include <algorithm>
#include <system_error>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>

namespace orion
{
namespace net
{

std::string AddressV6::to_string() const
{
   char buffer[256];

   RtlIpv6AddressToStringA(&_a, buffer); 

   return std::string(buffer);
}

AddressV6 AddressV6::parse(const std::string& s)
{
   AddressV6 addr;
   uint16_t port = 0;

   auto str = utf8_to_wstring(s);

   RtlIpv6StringToAddressExW(str.data(), &addr._a, (unsigned long *)&addr._scope_id, &port);
   
   auto ec = std::error_code(::GetLastError(), std::system_category());
   if (ec)
      throw std::system_error(ec);

   return addr;
}

} // net
} // orion
