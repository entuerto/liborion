/*
 * IPv4-win32.cpp
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

#include <orion/net/IPv4.h>

#include <orion/StringUtils.h>

#include <winsock2.h>
#include <ws2tcpip.h>

namespace orion
{
namespace net
{

uint32_t IPv4::to_ulong() const
{
   return ntohl(_a.l);
}

IPv4 IPv4::parse(const std::string& s)
{
   union
   {
      sockaddr base;
      sockaddr_in v4;
   } address;

   int address_length = sizeof(address);

   auto ipw = utf8_to_wstring(s);

   address.v4.sin_family = AF_INET;

   int rc = WSAStringToAddressW(
      const_cast<wchar_t*>(ipw.data()), AF_INET, 0, &address.base, &address_length);

   // Success return IPv4 address
   if (rc == 0)
   {
      return IPv4{address.v4.sin_addr.S_un.S_un_b.s_b1,
                  address.v4.sin_addr.S_un.S_un_b.s_b2,
                  address.v4.sin_addr.S_un.S_un_b.s_b3,
                  address.v4.sin_addr.S_un.S_un_b.s_b4};
   }

   return IPv4();
}

} // net
} // orion
