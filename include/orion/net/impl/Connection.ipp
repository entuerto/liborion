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

template<typename SocketT>
Connection<SocketT>::Connection(SocketT socket)
   : _local_endpoint()
   , _remote_endpoint()
   , _socket(std::move(socket))
   , _read_timeout()
   , _write_timeout()
   , _read_timeout_timer(_socket.get_executor().context())
   , _write_timeout_timer(_socket.get_executor().context())
{
}

template<typename SocketT>
Connection<SocketT>::~Connection()
{
   close();
}

template<typename SocketT>
void Connection<SocketT>::close()
{
   LOG(Info) << fmt::format("({:#x}) Connection closed", reinterpret_cast<uintptr_t>(this));

   std::error_code ec;

   // Send a TCP shutdown
   _socket.shutdown(asio::ip::tcp::socket::shutdown_send, ec);
   if (ec)
      log::error(ec, _src_loc);

   read_timeout_timer().cancel();
   write_timeout_timer().cancel();
}

template<typename SocketT>
const EndPoint& Connection<SocketT>::local_endpoint() const
{
   return _local_endpoint;
}

template<typename SocketT>
void Connection<SocketT>::local_endpoint(const EndPoint& value)
{
   _local_endpoint = value;
}

template<typename SocketT>
const EndPoint& Connection<SocketT>::remote_endpoint() const
{
   return _remote_endpoint;
}

template<typename SocketT>
void Connection<SocketT>::remote_endpoint(const EndPoint& value)
{
   _remote_endpoint = value;
}

template<typename SocketT>
std::error_code Connection<SocketT>::timeouts(const std::chrono::seconds& sec)
{
   _read_timeout  = sec;
   _write_timeout = sec;
   return std::error_code();
}

template<typename SocketT>
std::chrono::seconds Connection<SocketT>::timeouts() const
{
   return _read_timeout;
}

template<typename SocketT>
std::error_code Connection<SocketT>::read_timeout(const std::chrono::seconds& sec)
{
   _read_timeout = sec;
   return std::error_code();
}

template<typename SocketT>
std::chrono::seconds Connection<SocketT>::read_timeout() const
{
   return _read_timeout;
}

template<typename SocketT>
std::error_code Connection<SocketT>::write_timeout(const std::chrono::seconds& sec)
{
   _write_timeout = sec;
   return std::error_code();
}

template<typename SocketT>
std::chrono::seconds Connection<SocketT>::write_timeout() const
{
   return _write_timeout;
}

template<typename SocketT>
void Connection<SocketT>::on_read_timeout(const std::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "Connection::on_read_timeout()")

   log::debug(ec);

   close();
}

template<typename SocketT>
void Connection<SocketT>::on_write_timeout(const std::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "Connection::on_write_timeout()")

   log::debug(ec);

   close();
}

template<typename SocketT>
void Connection<SocketT>::start_read_timer()
{
   if (read_timeout() == std::chrono::seconds::zero())
      return;

   _read_timeout_timer.expires_after(read_timeout());

   _read_timeout_timer.async_wait(
      std::bind(&Connection::on_read_timeout, this, std::placeholders::_1));
}

template<typename SocketT>
void Connection<SocketT>::start_write_timer()
{
   if (write_timeout() == std::chrono::seconds::zero())
      return;

   _write_timeout_timer.expires_after(write_timeout());

   _write_timeout_timer.async_wait(
      std::bind(&Connection::on_write_timeout, this, std::placeholders::_1));
}

template<typename SocketT>
asio::steady_timer& Connection<SocketT>::read_timeout_timer()
{
   return _read_timeout_timer;
}

template<typename SocketT>
asio::steady_timer& Connection<SocketT>::write_timeout_timer()
{
   return _write_timeout_timer;
}

template<typename SocketT>
SocketT& Connection<SocketT>::socket()
{
   return _socket;
}

template<typename SocketT>
void Connection<SocketT>::accept()
{
   local_endpoint(convert(_socket.local_endpoint()));
   remote_endpoint(convert(_socket.remote_endpoint()));

   LOG(Info) << fmt::format("({:#x}) Connection accepted", reinterpret_cast<uintptr_t>(this));
   LOG(Info) << "   Remote address: " << to_string(remote_endpoint());
   LOG(Info) << "   Local address:  " << to_string(local_endpoint());

   if (log::default_logger().is_enabled(log::Level::Debug))
      dump_socket_options();

   do_accept();

   do_read();
}

template<typename SocketT>
void Connection<SocketT>::dump_socket_options()
{
   static const std::string text = R"(
Socket options:
   keep alive          : {}
   no delay            : {}
   linger              : {}, timeout: {}
   reuse address       : {}
   receive buffer size : {}
   send buffer size    : {}
)";

   asio::ip::tcp::socket::keep_alive keep_alive_option;
   _socket.get_option(keep_alive_option);

   asio::ip::tcp::no_delay no_delay_option;
   _socket.get_option(no_delay_option);

   asio::socket_base::linger linger_option;
   _socket.get_option(linger_option);

   asio::socket_base::reuse_address reuse_address_option;
   _socket.get_option(reuse_address_option);

   asio::socket_base::receive_buffer_size recv_buf_size_option;
   _socket.get_option(recv_buf_size_option);

   asio::socket_base::send_buffer_size send_buf_size_option;
   _socket.get_option(send_buf_size_option);

   log::write(fmt::format(text,
                          keep_alive_option.value(),
                          no_delay_option.value(),
                          linger_option.enabled(),
                          linger_option.timeout(),
                          reuse_address_option.value(),
                          recv_buf_size_option.value(),
                          send_buf_size_option.value()));
}

//--------------------------------------------------------------------------------------------------
template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const KeepAlive& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::keep_alive(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Broadcast& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::broadcast(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Debug& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::debug(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const DoNotRoute& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::do_not_route(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const EnableConnectionAborted& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::enable_connection_aborted(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const Linger& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::linger(value, value.timeout), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReuseAddress& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::reuse_address(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReceiveBufferSize& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::receive_buffer_size(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const ReceiveLowWatermark& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::receive_low_watermark(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const SendBufferSize& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::send_buffer_size(value), ec);
   return ec;
}

template<typename SocketT>
inline std::error_code set_option(Connection<SocketT>& conn, const SendLowWatermark& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::socket_base::send_low_watermark(value), ec);
   return ec;
}

} // namespace net
} // namespace orion
#endif // ORION_CONNECTION_IPP