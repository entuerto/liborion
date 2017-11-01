//
// Connection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/tcp/Connection.h>

#include <orion/Log.h>
#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>

#include <boost/format.hpp>

using namespace std::chrono_literals;

namespace orion
{
namespace net
{
namespace tcp
{
//--------------------------------------------------------------------------------------------------
EndPoint convert(const asio::ip::tcp::endpoint& ep)
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

Connection::Connection(asio::io_service& io_service)
   : net::Connection(io_service)
   , _socket(io_service)
   
{
}

Connection::~Connection()
{
   close();
}

void Connection::close()
{
   LOG(Info) << boost::format("(%p) Connection closed") % this;

   std::error_code ec;

   _socket.close(ec);
   if (ec)
      log::error(ec);

   read_deadline_timer().cancel();
   write_deadline_timer().cancel();
}

std::error_code Connection::set_option(const NoDelay& value)
{
   asio::error_code ec;
   _socket.set_option(asio::ip::tcp::no_delay(value), ec);
   return ec;
}

std::error_code Connection::set_option(const KeepAlive& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::keep_alive(value), ec);
   return ec;
}

std::error_code Connection::set_option(const Broadcast& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::broadcast(value), ec);
   return ec;
}

std::error_code Connection::set_option(const Debug& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::debug(value), ec);
   return ec;
}

std::error_code Connection::set_option(const DoNotRoute& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::do_not_route(value), ec);
   return ec;
}

std::error_code Connection::set_option(const EnableConnectionAborted& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::enable_connection_aborted(value), ec);
   return ec;
}

std::error_code Connection::set_option(const Linger& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::linger(value, value.timeout), ec);
   return ec;
}

std::error_code Connection::set_option(const ReuseAddress& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::reuse_address(value), ec);
   return ec;
}

std::error_code Connection::set_option(const ReceiveBufferSize& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::receive_buffer_size(value), ec);
   return ec;
}

std::error_code Connection::set_option(const ReceiveLowWatermark& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::receive_low_watermark(value), ec);
   return ec;
}

std::error_code Connection::set_option(const SendBufferSize& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::send_buffer_size(value), ec);
   return ec;
}

std::error_code Connection::set_option(const SendLowWatermark& value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::send_low_watermark(value), ec);
   return ec;
}

TcpSocket& Connection::socket()
{
   return _socket;
}

void Connection::accept()
{
   local_endpoint(convert(_socket.local_endpoint()));
   remote_endpoint(convert(_socket.remote_endpoint()));

   LOG(Info) << boost::format("(%p) Connection accepted") % this;
   LOG(Info) << "   Remote address: " << to_string(remote_endpoint());
   LOG(Info) << "   Local address:  " << to_string(local_endpoint());

   if (log::default_logger().is_enabled(log::Level::Debug))
      dump_socket_options();

   start_read_timer();

   do_read();
}

void Connection::dump_socket_options()
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

} // tcp
} // net
} // orion
