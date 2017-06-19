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

#ifndef ORION_NET_CONNECTION_H
#define ORION_NET_CONNECTION_H

#include <chrono>
#include <system_error>

#include <orion/Orion-Stddefs.h>

#include <orion/net/IPAddress.h>

namespace orion
{
namespace net
{
   
//! This class provides a generic network connection
/*!
   Connection is a generic stream-oriented network connection.
 */
class API_EXPORT Connection 
{
public:
   Connection();
   virtual ~Connection();

   /// Close closes the connection.
   /// Any blocked Read or Write operations will be unblocked and return errors.
   virtual void close() =0;

   /// Returns the local network address.
   virtual IPAddress* local_addr() const;

   /// Returns the remote network address.
   virtual IPAddress* remote_addr() const;

   /// Sets the read and write deadlines associated
   /// with the connection. It is equivalent to calling both
   /// set_read_deadline and set_write_deadline.
   ///
   /// A deadline is an absolute time after which I/O operations
   /// fail with a timeout instead of blocking.
   ///
   /// A zero value for t means I/O operations will not time out.
   virtual std::error_code deadline(const std::chrono::seconds& sec);

   /// Get the current value of the read and write deadline.
   virtual std::chrono::seconds deadline() const;

   /// Sets the deadline for future Read calls.
   /// A zero value for t means Read will not time out.
   virtual std::error_code read_deadline(const std::chrono::seconds& sec);

   /// Get the current value of the read deadline.
   virtual std::chrono::seconds read_deadline() const;

   /// Sets the deadline for future Write calls.
   /// A zero value for t means Write will not time out.
   virtual std::error_code write_deadline(const std::chrono::seconds& sec);

   /// Get the current value of the write deadline.
   virtual std::chrono::seconds write_deadline() const;

protected:
   std::unique_ptr<IPAddress> _local_addr;
   std::unique_ptr<IPAddress> _remote_addr;

private:
   std::chrono::seconds _read_deadline;
   std::chrono::seconds _write_deadline;
};

} // net
} // orion
#endif 
