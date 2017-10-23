//
// ServerConnection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/tcp/ServerConnection.h>

#include <orion/Log.h>
#include <orion/net/IPv4.h>
#include <orion/net/IPv6.h>

#include <boost/format.hpp>

using namespace std::chrono_literals;

namespace orion
{
namespace net
{
namespace tcp
{
//--------------------------------------------------------------------------------------------------
IPAddress* convert(const asio::ip::tcp::endpoint& ep)
{
   int port  = ep.port();
   auto addr = ep.address();

   if (addr.is_v4())
   {
      auto addr_v4 = addr.to_v4();
      return new IPAddress(IPv4(addr_v4.to_bytes()), port);
   }

   if (addr.is_v6())
   {
      auto addr_v6 = addr.to_v6();
      return new IPAddress(IPv6(addr_v6.to_bytes()), port);
   }

   return nullptr;
}

//--------------------------------------------------------------------------------------------------

ServerConnection::ServerConnection(asio::io_service& io_service, const Handler& handler)
   : _socket(io_service)
   , _handler(handler)
   , _read_deadline(io_service)
   , _write_deadline(io_service)
   , _in_buffer_size(8192)
{
}

ServerConnection::~ServerConnection()
{
   close();
}

void ServerConnection::close()
{
   LOG(Info) << boost::format("(%p) Connection closed") % this;

   std::error_code ec;

   _socket.close(ec);
   if (ec)
      log::error(ec);

   _read_deadline.cancel();
   _write_deadline.cancel();
}

std::error_code ServerConnection::keep_alive(bool value)
{
   asio::error_code ec;
   _socket.set_option(asio::socket_base::keep_alive(value), ec);
   return ec;
}

bool ServerConnection::keep_alive() const
{
   asio::socket_base::keep_alive option;
   asio::error_code ec;
   _socket.get_option(option, ec);
   if (ec)
   {
      log::error(ec);
      return false;
   }
   return option.value();
}

std::error_code ServerConnection::no_delay(bool value)
{
   asio::error_code ec;
   _socket.set_option(asio::ip::tcp::no_delay(value), ec);
   return ec;
}

bool ServerConnection::no_delay() const
{
   asio::ip::tcp::no_delay option;
   asio::error_code ec;
   _socket.get_option(option, ec);
   if (ec)
   {
      log::error(ec);
      return false;
   }
   return option.value();
}

std::error_code ServerConnection::on_handler(std::streambuf* in, std::streambuf* out)
{
   return _handler(in, out);
}

asio::ip::tcp::socket& ServerConnection::socket()
{
   return _socket;
}

void ServerConnection::accept()
{
   _local_addr.reset(convert(_socket.local_endpoint()));
   _remote_addr.reset(convert(_socket.remote_endpoint()));

   LOG(Info) << boost::format("(%p) Connection accepted") % this;
   LOG(Info) << "   Remote address: " << _remote_addr->to_string();
   LOG(Info) << "   Local address:  " << _local_addr->to_string();

   if (log::default_logger().is_enabled(log::Level::Debug))
      dump_socket_options();

   start_read_timer();

   do_read();
}

void ServerConnection::start_read_timer()
{
   if (read_deadline() == std::chrono::seconds::zero())
      return;

   _read_deadline.expires_from_now(read_deadline());

   _read_deadline.async_wait(std::bind(
      &ServerConnection::on_read_timeout, this->shared_from_this(), std::placeholders::_1));
}

void ServerConnection::start_write_timer()
{
   if (write_deadline() == std::chrono::seconds::zero())
      return;

   _write_deadline.expires_from_now(write_deadline());

   _write_deadline.async_wait(std::bind(
      &ServerConnection::on_write_timeout, this->shared_from_this(), std::placeholders::_1));
}

void ServerConnection::do_read()
{
   auto self = this->shared_from_this();

   _socket.async_read_some(
      _in_streambuf.prepare(_in_buffer_size), [this, self](std::error_code ec, std::size_t bytes_transferred) {
         if (ec)
         {
            log::error(ec);
            close();
            return;
         }
         log::debug2("ServerConnection::do_read() ", int(bytes_transferred));

         _in_streambuf.commit(bytes_transferred);

         ec = on_handler(&_in_streambuf, &_out_streambuf);
         if (ec)
         {
            log::error(ec);
            close();
            return;
         }
         do_write();

         do_read();
      });
}

void ServerConnection::do_write()
{
   auto self = this->shared_from_this();

   asio::async_write(
      _socket, _out_streambuf.data(), [this, self](std::error_code ec, std::size_t bytes_written) {
         if (ec)
         {
            log::error(ec);
            close();
            return;
         }
         std::size_t bytes_to_write = _out_streambuf.size();

         _out_streambuf.consume(bytes_written);

         log::debug2("ServerConnection::do_write() ", int(bytes_to_write), " ", int(bytes_written));

         if (bytes_to_write == bytes_written)
            return;

         do_write();
      });
}

void ServerConnection::on_read_timeout(const asio::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "ServerConnection::on_read_timeout()")

   log::debug(ec);

   close();
}

void ServerConnection::on_write_timeout(const asio::error_code& ec)
{
   if (ec == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "ServerConnection::on_write_timeout()")

   log::debug(ec);

   close();
}

void ServerConnection::dump_socket_options()
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
