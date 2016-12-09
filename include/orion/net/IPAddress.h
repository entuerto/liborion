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
#include <orion/net/IP.h>

namespace orion
{
namespace net
{

/// IPAddress represents the address of an IP end point.
class API_EXPORT IPAddress 
{
public:
   IPAddress(const IPAddress& Other);
   IPAddress(IPAddress&& Other);
   virtual ~IPAddress();

   IPAddress& operator=(const IPAddress& Rhs);
   IPAddress& operator=(IPAddress&& Rhs);

   virtual IP* ip() const;

   virtual std::string zone() const;

   virtual std::string to_string() const;

protected:
   IPAddress(IP* ip, const std::string& zone);

private:
   std::unique_ptr<IP> _ip;
   std::string _zone;
};

/// TcpAddress represents the address of a TCP end point. 
class API_EXPORT TcpAddress : public IPAddress 
{
public:
   TcpAddress(IP* ip, int port, const std::string& zone = "");

   TcpAddress(const TcpAddress& Other);
   TcpAddress(TcpAddress&& Other);

   ~TcpAddress();

   TcpAddress& operator=(const TcpAddress& Rhs);
   TcpAddress& operator=(TcpAddress&& Rhs);

   int port() const;

   virtual std::string to_string() const override;

public:
   /// Create a network end point address
   static std::unique_ptr<TcpAddress> create(const IP& ip, int port);
   /// Create a network end point address
   static std::unique_ptr<TcpAddress> create_v4(const std::string& ip, int port);
   /// Create a network end point address
   static std::unique_ptr<TcpAddress> create_v6(const std::string& ip, int port);

private:
   int _port;
};

} // net
} // orion
#endif 
