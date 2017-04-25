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

#include <orion/Logging.h>
#include <orion/StringUtils.h>

using namespace orion::logging;

namespace orion
{
namespace net
{

namespace priv
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

} // namespace priv

//---------------------------------------------------------------------------------------
// IPAddress

IPAddress::IPAddress(const IPv4& ip, int port):
   _ip(std::make_unique<IPv4>(ip)),
   _zone(),
   _port(port)
{
}

IPAddress::IPAddress(const IPv6& ip, int port, const std::string& zone):
   _ip(std::make_unique<IPv6>(ip)),
   _zone(zone),
   _port(port)
{
}

IPAddress::IPAddress(const IPAddress& Other):
   _ip(priv::clone_ip(Other._ip.get())), 
   _zone(Other._zone), 
   _port(Other._port) 
{
}

IPAddress::IPAddress(IPAddress&& Other):
   _ip(std::move(Other._ip)), 
   _zone(std::move(Other._zone)), 
   _port(std::move(Other._port)) 
{
}

IPAddress::~IPAddress() = default;

IPAddress& IPAddress::operator=(const IPAddress& Rhs) 
{
   if (this == &Rhs)
      return *this;

   _ip = std::unique_ptr<IP>(priv::clone_ip(Rhs._ip.get()));
   _zone = Rhs._zone;
   _port = Rhs._port;
   return *this;
}

IPAddress& IPAddress::operator=(IPAddress&& Rhs) 
{
   _ip = std::move(Rhs._ip);
   _zone = std::move(Rhs._zone);
   _port = std::move(Rhs._port);
   return *this;
}

IP* IPAddress::ip() const
{
   return _ip.get();
}

int IPAddress::port() const
{
   return _port;
}

std::string IPAddress::zone() const
{
   return _zone;
}

std::string IPAddress::to_string() const
{
   return _ip->to_string() + ":" + std::to_string(_port);
}

} // net
} // orion
