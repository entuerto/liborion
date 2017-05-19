/*
 * IPv6.h
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

#ifndef ORION_NET_IPV6_H
#define ORION_NET_IPV6_H

#include <orion/Orion-Stddefs.h>
#include <orion/net/IP.h>
#include <array>
#include <string>

namespace orion
{
namespace net
{

class API_EXPORT IPv6 : public IP
{
public:
   IPv6();
   IPv6(const std::array<uint8_t, 16>& b);
   IPv6(std::initializer_list<uint8_t> l);
   IPv6(const IPv6& Other);
   IPv6(IPv6&& Other);
   virtual ~IPv6();

   IPv6& operator=(const IPv6& Rhs);
   IPv6& operator=(IPv6&& Rhs);

   IPv6& operator=(std::initializer_list<uint8_t> l);

   bool operator==(const IPv6& Rhs) const;

   /// Reports whether ip is a loopback address.
   virtual bool is_loopback() const override;
   /// Reports whether ip is a multicast address.
   virtual bool is_multicast() const override;
   /// Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const override;

   /// Get the address in bytes, in network byte order.
   virtual uint8_t* to_bytes() const override;
   /// Returns the string form of the IP address
   virtual std::string to_string() const override;

   static IPv6 parse(const std::string& s);

public:
   static IPv6 zero;
   static IPv6 unspecified;
   static IPv6 loopback;
   static IPv6 interface_local_all_nodes;
   static IPv6 link_local_all_nodes;
   static IPv6 link_local_all_routers;

private:
   union
   {
      uint8_t b[16];
      uint16_t w[8];
   } _a;
};

} // net
} // orion
#endif // ORION_NET_IPV6_H
