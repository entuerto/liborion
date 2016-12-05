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
   virtual ~Connection() = default;

   /// Close closes the connection.
   /// Any blocked Read or Write operations will be unblocked and return errors.
   virtual void close() =0;

   /// Returns the local network address.
   virtual IPAddress* local_addr() const =0;

   /// Returns the remote network address.
   virtual IPAddress* remote_addr() const =0;

   /// Sets the read and write deadlines associated
   /// with the connection. It is equivalent to calling both
   /// set_read_deadline and set_write_deadline.
   ///
   /// A deadline is an absolute time after which I/O operations
   /// fail with a timeout instead of blocking.
   ///
   /// A zero value for t means I/O operations will not time out.
   virtual std::error_code deadline(const std::chrono::seconds& sec) =0;

   /// Get the current value of the read and write deadline.
   virtual std::chrono::seconds deadline() const =0;

   /// Sets the deadline for future Read calls.
   /// A zero value for t means Read will not time out.
   virtual std::error_code read_deadline(const std::chrono::seconds& sec) =0;

   /// Get the current value of the read deadline.
   virtual std::chrono::seconds read_deadline() const =0;

   /// Sets the deadline for future Write calls.
   /// A zero value for t means Write will not time out.
   virtual std::error_code write_deadline(const std::chrono::seconds& sec) =0;

   /// Get the current value of the write deadline.
   virtual std::chrono::seconds write_deadline() const =0;
};

/// TCP network connections.
class API_EXPORT TcpConnection : public Connection 
{
public:
   NO_COPY(TcpConnection)
   
   TcpConnection();
   virtual ~TcpConnection();

   virtual IPAddress* local_addr() const override;

   virtual IPAddress* remote_addr() const override;

   virtual std::error_code deadline(const std::chrono::seconds& sec) override;

   virtual std::chrono::seconds deadline() const override;

   virtual std::error_code read_deadline(const std::chrono::seconds& sec) override;

   virtual std::chrono::seconds read_deadline() const override;

   virtual std::error_code write_deadline(const std::chrono::seconds& sec) override;

   virtual std::chrono::seconds write_deadline() const override;

   /// Sets whether the operating system should send keepalive messages on the connection.
   virtual std::error_code keep_alive(bool value);

   /// Get the current value of the keepalive property.
   virtual bool keep_alive() const;

   /// Controls whether the operating system should delay packet transmission in hopes of 
   /// sending fewer packets (Nagle's algorithm). The default is true (no delay), meaning 
   /// that data is sent as soon as possible after a Write.
   virtual std::error_code no_delay(bool value);

   /// Get the current value of the nodelay property.
   virtual bool no_delay() const;

protected:
   std::unique_ptr<IPAddress> _local_addr;
   std::unique_ptr<IPAddress> _remote_addr;

private:
   std::chrono::seconds _read_deadline;
   std::chrono::seconds _write_deadline;
   bool _keep_alive;
   bool _no_delay;
};

} // net
} // orion
#endif 
