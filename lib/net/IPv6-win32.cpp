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

#include <orion/net/IPv6.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <algorithm>

#include <orion/StringUtils.h>

namespace orion
{
namespace net
{

IPv6 IPv6::parse(const std::string& s)
{
   union
   {
      sockaddr base;
      sockaddr_in6 v6;
   } address;

   int address_length = sizeof(address);

   auto ipw = utf8_to_wstring(s);

   address.v6.sin6_family = AF_INET6;

   int rc = WSAStringToAddressW(const_cast<wchar_t*>(ipw.data()), AF_INET6, 0, &address.base, &address_length);
   if (rc == 0)
   {
      IPv6 ip;

      auto s = std::begin(address.v6.sin6_addr.s6_bytes);
      auto e = std::end(address.v6.sin6_addr.s6_bytes);
      
      std::copy(s, e, std::begin(ip._a.b));
      return ip;
   }

   return IPv6();
}

} // net
} // orion
