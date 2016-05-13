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

namespace orion
{
namespace net
{
//---------------------------------------------------------------------------------------
// IPAddress

IPAddress::IPAddress(IP* ip, const std::string& zone):
   _ip(ip),
   _zone(zone)
{

}

IPAddress::~IPAddress()
{

}

IP::WeakPtr IPAddress::ip() const
{
   return _ip;
}

std::string IPAddress::zone() const
{
   return _zone;
}

//---------------------------------------------------------------------------------------
// TcpAddress

TcpAddress::TcpAddress(IP* ip, int port, const std::string& zone):
   IPAddress(ip, zone),
   _port(port)
{
}

TcpAddress::~TcpAddress()
{
}

int TcpAddress::port() const
{
   return _port;
}

} // net
} // orion
