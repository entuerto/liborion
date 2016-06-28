/*
 * IP.h
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

#ifndef ORION_NET_IP_H
#define ORION_NET_IP_H

#include <array>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>

namespace orion
{
namespace net
{

//! IPAddress represents the address of an IP end point.
class API_EXPORT IP {
public:
   DECLARE_POINTERS(IP)
   
   virtual ~IP() {}

   //! Reports whether ip is a loopback address.
   virtual bool is_loopback() const =0;
   //! Reports whether ip is a multicast address.
   virtual bool is_multicast() const =0;
   //! Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const =0;
   //! Returns the string form of the IP address
   virtual std::string to_string() const =0;
};

class API_EXPORT IPv4 : public IP {
public:
   IPv4();
   IPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
   IPv4(const IPv4& Other);
   IPv4(IPv4&& Other);
   virtual ~IPv4();

   IPv4& operator=(const IPv4& Rhs);
   IPv4& operator=(IPv4&& Rhs);

   bool operator==(const IPv4& Rhs) const;

   //! Reports whether ip is a loopback address.
   virtual bool is_loopback() const override;
   //! Reports whether ip is a multicast address.
   virtual bool is_multicast() const override;
   //! Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const override;
   //! Returns the string form of the IP address
   virtual std::string to_string() const override;

public:
   static IPv4 broadcast;
   static IPv4 all_systems;
   static IPv4 all_routers;
   static IPv4 zero;

private:
   std::array<uint8_t, 4> _a;
};

class API_EXPORT IPv6 : public IP {
public:
   IPv6();
   IPv6(const std::array<uint8_t, 16>& a);
   IPv6(const IPv6& Other);
   IPv6(IPv6&& Other);
   virtual ~IPv6();

   IPv6& operator=(const IPv6& Rhs);
   IPv6& operator=(IPv6&& Rhs);

   bool operator==(const IPv6& Rhs) const;

   //! Reports whether ip is a loopback address.
   virtual bool is_loopback() const override;
   //! Reports whether ip is a multicast address.
   virtual bool is_multicast() const override;
   //! Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const override;
   //! Returns the string form of the IP address
   virtual std::string to_string() const override;

public:
   static IPv6 zero;
   static IPv6 unspecified;
   static IPv6 loopback;
   static IPv6 interface_local_all_nodes;
   static IPv6 link_local_all_nodes;
   static IPv6 link_local_all_routers;

private:
   std::array<uint8_t, 16> _a;
};

} // net
} // orion
#endif 
