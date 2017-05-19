/*
 * IPv4.h
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

#ifndef ORION_NET_IPV4_H
#define ORION_NET_IPV4_H

#include <orion/Orion-Stddefs.h>
#include <orion/net/IP.h>
#include <array>
#include <string>

namespace orion
{
namespace net
{

class API_EXPORT IPv4 : public IP
{
public:
   IPv4();
   IPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
   IPv4(const std::array<uint8_t, 4>& b);
   IPv4(std::initializer_list<uint8_t> l);
   IPv4(const IPv4& Other);
   IPv4(IPv4&& Other);
   virtual ~IPv4();

   IPv4& operator=(const IPv4& Rhs);
   IPv4& operator=(IPv4&& Rhs);

   IPv4& operator=(std::initializer_list<uint8_t> l);

   bool operator==(const IPv4& Rhs) const;

   /// Determine whether the address is a class A address.
   bool is_class_a() const;
   /// Determine whether the address is a class B address.
   bool is_class_b() const;
   /// Determine whether the address is a class C address.
   bool is_class_c() const;

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

   /// Get the address as an unsigned long in host byte order.
   uint32_t to_ulong() const;

   static IPv4 parse(const std::string& s);

public:
   static IPv4 broadcast;
   static IPv4 all_systems;
   static IPv4 all_routers;
   static IPv4 zero;

private:
   union
   {
      uint8_t b[4];
      uint16_t w[2];
      uint32_t l;
   } _a;
};

} // net
} // orion
#endif // ORION_NET_IPV4_H
