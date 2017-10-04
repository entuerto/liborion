/*
 * IPAddress.h
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

#ifndef ORION_NET_IPADDRESS_H
#define ORION_NET_IPADDRESS_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/IP.h>
#include <orion/net/IPv4.h>
#include <orion/net/IPv6.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{

/// IPAddress represents the address of an IP end point.
class API_EXPORT IPAddress
{
public:
   IPAddress(const IPv4& ip, int port);
   IPAddress(const IPv6& ip, int port, const std::string& zone = "");
   IPAddress(const IPAddress& Other);
   IPAddress(IPAddress&& Other);
   virtual ~IPAddress();

   IPAddress& operator=(const IPAddress& Rhs);
   IPAddress& operator=(IPAddress&& Rhs);

   virtual IP* ip() const;

   virtual int port() const;

   virtual std::string zone() const;

   virtual std::string to_string() const;

private:
   std::unique_ptr<IP> _ip;
   std::string _zone;
   int _port;
};

} // net
} // orion
#endif
