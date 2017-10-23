/*
 * Connection.h
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

#ifndef ORION_NET_TCP_CONNECTION_H
#define ORION_NET_TCP_CONNECTION_H

#include <orion/Orion-Stddefs.h>
#include <orion/net/Connection.h>

#include <iosfwd>
#include <system_error>

namespace orion
{
namespace net
{
namespace tcp
{
/// TCP network connections.
class API_EXPORT Connection : public net::Connection
{
public:
   virtual ~Connection() = default;

   /// Sets whether the operating system should send keepalive messages on the connection.
   virtual std::error_code keep_alive(bool value) =0;

   /// Get the current value of the keepalive property.
   virtual bool keep_alive() const =0;

   /// Controls whether the operating system should delay packet transmission in hopes of
   /// sending fewer packets (Nagle's algorithm). The default is true (no delay), meaning
   /// that data is sent as soon as possible after a Write.
   virtual std::error_code no_delay(bool value) =0;

   /// Get the current value of the nodelay property.
   virtual bool no_delay() const =0;

   virtual std::error_code on_handler(std::streambuf* in, std::streambuf* out) =0;

};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_CONNECTION_H
