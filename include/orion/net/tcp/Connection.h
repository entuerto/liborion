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

#include <orion/detail/AsyncTypes.h>
#include <orion/net/Connection.h>
#include <orion/net/tcp/Utils.h>

#include <iosfwd>
#include <system_error>

namespace orion
{
namespace net
{
namespace tcp
{
/// TCP network connections.
class API_EXPORT Connection 
   : public net::Connection
   , public std::enable_shared_from_this<Connection>
{
public:
   Connection(IOService& io_service);
   virtual ~Connection();

   void close() override;

   /// Controls whether the operating system should delay packet transmission in hopes of
   /// sending fewer packets (Nagle's algorithm). The default is true (no delay), meaning
   /// that data is sent as soon as possible after a Write.
   std::error_code set_option(const NoDelay& value);

   /// Sets whether the operating system should send keepalive messages on the connection.
   std::error_code set_option(const KeepAlive& value);

   /// Socket option to permit sending of broadcast messages.
   std::error_code set_option(const Broadcast& value);

   /// Socket option to enable socket-level debugging.
   std::error_code set_option(const Debug& value);

   /// Socket option to prevent routing, use local interfaces only.
   std::error_code set_option(const DoNotRoute& value);

   /// Socket option to report aborted connections on accept.
   std::error_code set_option(const EnableConnectionAborted& value);

   /// Socket option to specify whether the socket lingers on close if unsent data is present.
   std::error_code set_option(const Linger& value);

   /// Socket option to allow the socket to be bound to an address that is already in use.
   std::error_code set_option(const ReuseAddress& value);

   /// Socket option for the receive buffer size of a socket.
   std::error_code set_option(const ReceiveBufferSize& value);

   /// Socket option for the receive low watermark.
   std::error_code set_option(const ReceiveLowWatermark& value);

   /// Socket option for the send buffer size of a socket.
   std::error_code set_option(const SendBufferSize& value);

   /// Socket option for the send low watermark.
   std::error_code set_option(const SendLowWatermark& value);

   TcpSocket& socket();

   virtual void accept();

protected:
   /// Perform an asynchronous read operation.
   virtual void do_read() =0;

   /// Perform an asynchronous write operation.
   virtual void do_write() =0;

private:
   void dump_socket_options();

   /// Socket for the connection.
   TcpSocket _socket;
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_CONNECTION_H
