//
// Connection.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_CONNECTION_H
#define ORION_NET_CONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/EndPoint.h>
#include <orion/net/Utils.h>

#include <asio.hpp>

#include <chrono>
#include <system_error>

namespace orion
{
namespace net
{
/// This class provides a generic network connection
///
/// Connection is a generic stream-oriented network connection.
///
template<typename SocketT>
class Connection : public std::enable_shared_from_this<Connection<SocketT>>
{
public:
   NO_COPY(Connection)

   Connection(asio::io_context& io_context);
   virtual ~Connection();

   /// Close closes the connection.
   /// Any blocked Read or Write operations will be unblocked and return errors.
   virtual void close();

   /// Returns the local network address.
   const EndPoint& local_endpoint() const;

   /// Set the local network address.
   void local_endpoint(const EndPoint& value);

   /// Returns the remote network address.
   const EndPoint& remote_endpoint() const;

   /// Set the remote network address.
   void remote_endpoint(const EndPoint& value);

   /// Sets the read and write deadlines associated
   /// with the connection. It is equivalent to calling both
   /// read_deadline and write_deadline.
   ///
   /// A deadline is an absolute time after which I/O operations
   /// fail with a timeout instead of blocking.
   ///
   /// A zero value for t means I/O operations will not time out.
   std::error_code deadline(const std::chrono::seconds& sec);

   /// Get the current value of the read and write deadline.
   std::chrono::seconds deadline() const;

   /// Sets the deadline for future Read calls.
   /// A zero value for t means Read will not time out.
   std::error_code read_deadline(const std::chrono::seconds& sec);

   /// Get the current value of the read deadline.
   std::chrono::seconds read_deadline() const;

   /// Sets the deadline for future Write calls.
   /// A zero value for t means Write will not time out.
   std::error_code write_deadline(const std::chrono::seconds& sec);

   /// Get the current value of the write deadline.
   std::chrono::seconds write_deadline() const;

   SocketT& socket();

   void accept();

   void start_read_timer();
   void start_write_timer();

   asio::steady_timer& read_deadline_timer();
   asio::steady_timer& write_deadline_timer();

protected:
   void dump_socket_options();

   /// Handle timeout
   void on_read_timeout(const std::error_code& ec);
   void on_write_timeout(const std::error_code& ec);

   /// Perform an asynchronous read operation.
   virtual void do_read() {}

   /// Perform an asynchronous write operation.
   virtual void do_write() {}

private:
   EndPoint _local_endpoint;
   EndPoint _remote_endpoint;

   /// Socket for the connection.
   SocketT _socket;

   std::chrono::seconds _read_deadline;
   std::chrono::seconds _write_deadline;

   asio::steady_timer _read_deadline_timer;
   asio::steady_timer _write_deadline_timer;
};

/// Sets whether the operating system should send keepalive messages on the connection.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const KeepAlive& value);

/// Socket option to permit sending of broadcast messages.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Broadcast& value);

/// Socket option to enable socket-level debugging.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Debug& value);

/// Socket option to prevent routing, use local interfaces only.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const DoNotRoute& value);

/// Socket option to report aborted connections on accept.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const EnableConnectionAborted& value);

/// Socket option to specify whether the socket lingers on close if unsent data is present.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Linger& value);

/// Socket option to allow the socket to be bound to an address that is already in use.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReuseAddress& value);

/// Socket option for the receive buffer size of a socket.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReceiveBufferSize& value);

/// Socket option for the receive low watermark.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReceiveLowWatermark& value);

/// Socket option for the send buffer size of a socket.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const SendBufferSize& value);

/// Socket option for the send low watermark.
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const SendLowWatermark& value);
} // net
} // orion

#include <orion/net/impl/Connection.ipp>

#endif
