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
#include <cstring>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>

namespace orion
{
namespace net
{

std::string to_string(const AddressV6& addr)
{
   wchar_t buffer[256];

   RtlIpv6AddressToStringW(&addr._a, buffer); 

   return wstring_to_utf8(buffer);
}

AddressV6 make_address_v6(const std::string& s)
{
   in6_addr addr;
   uint32_t scope_id = 0;
   uint16_t port = 0;

   // Get buffer size
   int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);

   wchar_t ws_buffer[len];

   MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, ws_buffer, len);

   RtlIpv6StringToAddressExW(ws_buffer, &addr, (PULONG)&scope_id, &port);
   
   auto ec = std::error_code(::GetLastError(), std::system_category());
   if (ec)
      throw std::system_error(ec);

   std::array<uint8_t, 16> b; 
   std::memcpy(b.data(), addr.s6_addr, 16);

   return AddressV6(b, scope_id);
}

} // net
} // orion
