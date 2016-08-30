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

std::unique_ptr<TcpAddress> TcpAddress::create(const IP& ip, int port)
{
   return std::make_unique<TcpAddress>(clone_ip(&ip), port);
}

std::unique_ptr<TcpAddress> TcpAddress::create(const std::string& ip, int port)
{
   int n, a, b, c, d;

   if (std::sscanf(ip.c_str(), "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) != 4)
      return nullptr;

   return TcpAddress::create(IPv4{a, b, c, d}, port);
}

} // net
} // orion
