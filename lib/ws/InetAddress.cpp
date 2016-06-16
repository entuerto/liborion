/*
 * Request.cpp
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

#include <orion/ws/InetAddress.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace orion
{
namespace ws
{

InetAddress::InetAddress(const std::string& address, int port) :
   _address(address),
   _port(port)
{
}

InetAddress::~InetAddress()
{
}

std::string InetAddress::address() const
{
   return _address;
}

int InetAddress::port() const
{
   return _port;
}

InetAddress::SharedPtr InetAddress::create(const std::string& address, int port)
{
   return InetAddress::SharedPtr(new InetAddress(address, port));
}

InetAddress::SharedPtr InetAddress::create(long ip, int port)
{
   struct in_addr ra;
   ra.s_addr = ip;
   
   return std::make_shared<InetAddress>(inet_ntoa(ra), port);
}

} // ws
} // orion

