/*
 * IPAddress.cpp
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

#include <orion/net/IPAddress.h>

#include <cstdio>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <orion/Logging.h>
#include <orion/StringUtils.h>

using namespace orion::logging;

namespace orion
{
namespace net
{

IP* clone_ip(const IP* ip)
{
   const IPv4* ip_v4 = dynamic_cast<const IPv4*>(ip);

   if (ip_v4 != nullptr)
      return new IPv4(*ip_v4);

   const IPv6* ip_v6 = dynamic_cast<const IPv6*>(ip);

   if (ip_v4 != nullptr)
      return new IPv6(*ip_v6);

   return nullptr;
}

//---------------------------------------------------------------------------------------
// IPAddress

IPAddress::IPAddress(const IPAddress& Other):
   _ip(clone_ip(Other._ip.get())), 
   _zone(Other._zone) 
{
}

IPAddress::IPAddress(IPAddress&& Other):
   _ip(std::move(Other._ip)), 
   _zone(std::move(Other._zone)) 
{
}

IPAddress::IPAddress(IP* ip, const std::string& zone):
   _ip(ip),
   _zone(zone)
{
}

IPAddress::~IPAddress() = default;

IPAddress& IPAddress::operator=(const IPAddress& Rhs) 
{
   if (this == &Rhs)
      return *this;

   _ip = std::unique_ptr<IP>(clone_ip(Rhs._ip.get()));
   _zone = Rhs._zone;
   return *this;
}

IPAddress& IPAddress::operator=(IPAddress&& Rhs) 
{
   _ip = std::move(Rhs._ip);
   _zone = std::move(Rhs._zone);
   return *this;
}

IP* IPAddress::ip() const
{
   return _ip.get();
}

std::string IPAddress::zone() const
{
   return _zone;
}

std::string IPAddress::to_string() const
{
   return _ip->to_string();
}

//---------------------------------------------------------------------------------------
// TcpAddress

TcpAddress::TcpAddress(IP* ip, int port, const std::string& zone /* = "" */):
   IPAddress(ip, zone),
   _port(port)
{
}

TcpAddress::TcpAddress(const TcpAddress& Other):
   IPAddress(Other), 
   _port(Other._port) 
{
}

TcpAddress::TcpAddress(TcpAddress&& Other):
   IPAddress(std::move(Other)), 
   _port(std::move(Other._port)) 
{
}

TcpAddress::~TcpAddress() = default;

TcpAddress& TcpAddress::operator=(const TcpAddress& Rhs) 
{
   if (this == &Rhs)
      return *this;

   IPAddress::operator=(Rhs);
   _port = Rhs._port;

   return *this;
}

TcpAddress& TcpAddress::operator=(TcpAddress&& Rhs) 
{
   IPAddress::operator=(std::move(Rhs));
   _port = std::move(Rhs._port);

   return *this;
}

int TcpAddress::port() const
{
   return _port;
}

std::string TcpAddress::to_string() const
{
   return IPAddress::to_string() + ":" + std::to_string(_port);
}

std::unique_ptr<TcpAddress> TcpAddress::create(const IP& ip, int port)
{
   return std::make_unique<TcpAddress>(clone_ip(&ip), port);
}

std::unique_ptr<TcpAddress> TcpAddress::create_v4(const std::string& ip, int port)
{
   union
   {
      sockaddr base;
      sockaddr_in v4;
   } address;

   int address_length = sizeof(address);

   auto ipw = utf8_to_wstring(ip);

   int rc = WSAStringToAddressW(const_cast<wchar_t*>(ipw.data()), AF_INET, 0, &address.base, &address_length);

   // Success return IPv4 address
   if (rc == 0)
   {
      return TcpAddress::create(IPv4{address.v4.sin_addr.S_un.S_un_b.s_b1, 
                                     address.v4.sin_addr.S_un.S_un_b.s_b2, 
                                     address.v4.sin_addr.S_un.S_un_b.s_b3, 
                                     address.v4.sin_addr.S_un.S_un_b.s_b4}, port);
   }
   
   return nullptr;
}

std::unique_ptr<TcpAddress> TcpAddress::create_v6(const std::string& ip, int port)
{
   union
   {
      sockaddr base;
      sockaddr_in6 v6;
   } address;

   int address_length = sizeof(address);

   auto ipw = utf8_to_wstring(ip);

   if (WSAStringToAddressW(const_cast<wchar_t*>(ipw.data()), AF_INET6, 0, &address.base, &address_length) == 0)
   {
      std::array<uint8_t, 16> b;
      std::copy(address.v6.sin6_addr.s6_bytes, address.v6.sin6_addr.s6_bytes + 16, b.begin());
      return TcpAddress::create(IPv6{b}, port);
   }

   auto err = std::error_code(WSAGetLastError(), std::system_category());
   LOG(Error) << err;

   return nullptr;
}

} // net
} // orion
