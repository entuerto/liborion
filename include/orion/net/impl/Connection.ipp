//
// Connection.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_CONNECTION_IPP
#define ORION_CONNECTION_IPP

#include <orion/Orion-Stddefs.h>

#include <orion/Log.h>
#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>
#include <orion/net/Utils.h>

#include <orion/detail/AsyncTypes.h>

#include <fmt/format.h>

using namespace std::chrono_literals;

namespace orion
{
namespace net
{

//--------------------------------------------------------------------------------------------------

inline EndPoint convert(const asio::ip::tcp::endpoint& ep)
{
   auto addr = ep.address();

   if (addr.is_v4())
   {
      auto addr_v4 = addr.to_v4();
      return EndPoint(AddressV4(addr_v4.to_bytes()), ep.port());
   }

   if (addr.is_v6())
   {
      auto addr_v6 = addr.to_v6();
      return EndPoint(AddressV6(addr_v6.to_bytes()), ep.port());
   }

   return EndPoint();
}

//--------------------------------------------------------------------------------------------------

template <typename SocketT>
Connection<SocketT>::Connection(asio::io_service& io_service)
   : _local_endpoint()
   , _remote_endpoint()
   , _socket(io_service)
   , _read_deadline()
   , _write_deadline()
   , _read_deadline_timer(io_service)
   , _write_deadline_timer(io_service)
{
}

template <typename SocketT>
Connection<SocketT>::~Connection()
{
   close();
}

template <typename SocketT>
void Connection<SocketT>::close()
{
   LOG(Info) << fmt::format("({:#x}) Connection closed", reinterpret_cast<uintptr_t>(this));

   std::error_code ec;

   _socket.close(ec);
   if (ec)
      log::error(ec);

   read_deadline_timer().cancel();
   write_deadline_timer().cancel();
}

template <typename SocketT>
const EndPoint& Connection<SocketT>::local_endpoint() const
{
   return _local_endpoint;
}

template <typename SocketT>
void Connection<SocketT>::local_endpoint(const EndPoint& value)
{
   _local_endpoint = value;
}

template <typename SocketT>
const EndPoint& Connection<SocketT>::remote_endpoint() const
{
   return _remote_endpoint;
}

template <typename SocketT>
void Connection<SocketT>::remote_endpoint(const EndPoint& value)
{
   _remote_endpoint = value;
}

template <typename SocketT>
std::error_code Connection<SocketT>::deadline(const std::chrono::seconds& sec)
{
   _read_deadline  = sec;
   _write_deadline = sec;
   return std::error_code();
}

template <typename SocketT>
std::chrono::seconds Connection<SocketT>::deadline() const
{
   return _read_deadline;
}

template <typename SocketT>
std::error_code Connection<SocketT>::read_deadline(const std::chrono::seconds& sec)
{
   _read_deadline = sec;
   return std::error_code();
}

template <typename SocketT>
std::chrono::seconds Connection<SocketT>::read_deadline() const
{
   return _read_deadline;
}

template <typename SocketT>
std::error_code Connection<SocketT>::write_deadline(const std::chrono::seconds& sec)
{
   _write_deadline = sec;
   return std::error_code();
}

template <typename SocketT>
std::chrono::seconds Connection<SocketT>::write_deadline() const
{
   return _write_deadline;
}

template <typename SocketT>
void Connection<SocketT>::on_read_timeout(const std::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "Connection::on_read_timeout()")

   log::debug(ec);

   close();
}

template <typename SocketT>
void Connection<SocketT>::on_write_timeout(const std::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "Connection::on_write_timeout()")

   log::debug(ec);

   close();
}

template <typename SocketT>
void Connection<SocketT>::start_read_timer()
{
   if (read_deadline() == std::chrono::seconds::zero())
      return;

   _read_deadline_timer.expires_from_now(read_deadline());

   _read_deadline_timer.async_wait(std::bind(
      &Connection::on_read_timeout, this, std::placeholders::_1));
}

template <typename SocketT>
void Connection<SocketT>::start_write_timer()
{
   if (write_deadline() == std::chrono::seconds::zero())
      return;

   _write_deadline_timer.expires_from_now(write_deadline());

   _write_deadline_timer.async_wait(std::bind(
      &Connection::on_write_timeout, this, std::placeholders::_1));
}

template <typename SocketT>
SteadyTimer& Connection<SocketT>::read_deadline_timer()
{
   return _read_deadline_timer;
}

template <typename SocketT>
SteadyTimer& Connection<SocketT>::write_deadline_timer()
{
   return _write_deadline_timer;
}

template <typename SocketT>
SocketT& Connection<SocketT>::socket()
{
   return _socket;
}

template <typename SocketT>
void Connection<SocketT>::accept()
{
   local_endpoint(convert(_socket.local_endpoint()));
   remote_endpoint(convert(_socket.remote_endpoint()));

   LOG(Info) << fmt::format("({:#x}) Connection accepted", reinterpret_cast<uintptr_t>(this));
   LOG(Info) << "   Remote address: " << to_string(remote_endpoint());
   LOG(Info) << "   Local address:  " << to_string(local_endpoint());

   if (log::default_logger().is_enabled(log::Level::Debug))
      dump_socket_options();

   start_read_timer();

   do_read();
}

template <typename SocketT>
void Connection<SocketT>::dump_socket_options()
{
   log::write("Socket options:");

   asio::ip::tcp::socket::keep_alive keep_alive_option;
   _socket.get_option(keep_alive_option);

   log::write("   keep alive          : ", (keep_alive_option.value() ? "true" : "false"));

   asio::ip::tcp::no_delay no_delay_option;
   _socket.get_option(no_delay_option);

   log::write("   no delay            : ", (no_delay_option.value() ? "true" : "false"));

   asio::socket_base::linger linger_option;
   _socket.get_option(linger_option);

   log::write("   linger              : ",
              (linger_option.enabled() ? "true" : "false"),
              ", timeout: ",
              linger_option.timeout());

   asio::socket_base::reuse_address reuse_address_option;
   _socket.get_option(reuse_address_option);

   log::write("   reuse address       : ", (reuse_address_option.value() ? "true" : "false"));

   asio::socket_base::receive_buffer_size recv_buf_size_option;
   _socket.get_option(recv_buf_size_option);

   log::write("   receive buffer size : ", recv_buf_size_option.value());

   asio::socket_base::send_buffer_size send_buf_size_option;
   _socket.get_option(send_buf_size_option);

   log::write("   send buffer size    : ", send_buf_size_option.value());
}

//--------------------------------------------------------------------------------------------------
template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const KeepAlive& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::keep_alive(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Broadcast& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::broadcast(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Debug& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::debug(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const DoNotRoute& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::do_not_route(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const EnableConnectionAborted& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::enable_connection_aborted(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Linger& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::linger(value, value.timeout), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReuseAddress& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::reuse_address(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReceiveBufferSize& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::receive_buffer_size(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReceiveLowWatermark& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::receive_low_watermark(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const SendBufferSize& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::send_buffer_size(value), ec);
   return ec;
}

template <typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const SendLowWatermark& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::send_low_watermark(value), ec);
   return ec;
}

} // namespace net
} // namespace orion
#endif // ORION_CONNECTION_IPP