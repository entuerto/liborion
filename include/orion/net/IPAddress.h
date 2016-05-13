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

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/net/IP.h>

namespace orion
{
namespace net
{

//! IPAddress represents the address of an IP end point.
class API_EXPORT IPAddress {
public:
	DECLARE_POINTERS(IPAddress)

   virtual ~IPAddress();

   virtual IP::WeakPtr ip() const;

   virtual std::string zone() const;

protected:
   IPAddress(IP* ip, const std::string& zone);

private:
   IP::SharedPtr _ip;
   std::string _zone;
};

//! TcpAddress represents the address of a TCP end point. 
class API_EXPORT TcpAddress : public IPAddress {
public:
   TcpAddress(IP* ip, int port, const std::string& zone = "");
   ~TcpAddress();

   int port() const;

private:
   int _port;
};

} // net
} // orion
#endif 
