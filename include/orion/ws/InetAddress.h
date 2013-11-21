/*
 * InetAddress.h
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

#ifndef ORION_WS_INETADDRESS_H
#define ORION_WS_INETADDRESS_H

#include <string>
#include <orion/MemoryUtils.h>

namespace orion
{
namespace ws
{
//! Represents a network address as an IP address and a port number.
class InetAddress
{
public:
   DECLARE_POINTERS(InetAddress)

   InetAddress(const std::string& address, int port);
   virtual ~InetAddress();

   std::string address() const;

   int port() const;


   static InetAddress::SharedPtr create(const std::string& address, int port);
   static InetAddress::SharedPtr create(long ip, int port);

private:
   std::string _address;
   int _port;

};

} // ws
} // orion
#endif

