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

#include <orion/Orion-Stddefs.h>

#include <array>
#include <string>

namespace orion
{
namespace net
{

/// IPAddress represents the address of an IP end point.
class API_EXPORT IP
{
public:
   virtual ~IP() = default;

   /// Reports whether ip is a loopback address.
   virtual bool is_loopback() const = 0;
   /// Reports whether ip is a multicast address.
   virtual bool is_multicast() const = 0;
   /// Reports whether ip is an unspecified address.
   virtual bool is_unspecified() const = 0;

   /// Get the address in bytes, in network byte order.
   virtual uint8_t* to_bytes() const = 0;
   /// Returns the string form of the IP address
   virtual std::string to_string() const = 0;
};

inline std::ostream& operator<<(std::ostream& o, const IP& ip)
{
   o << ip.to_string();
   return o;
}

inline std::ostream& operator<<(std::ostream& o, const IP* ip)
{
   o << (ip == nullptr ? "null" : ip->to_string());
   return o;
}

} // net
} // orion
#endif
