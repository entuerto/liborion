//
// AsioServerConnection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/impl/AsioServerConnection.h>

#include <orion/Log.h>
#include <orion/net/IPv4.h>
#include <orion/net/IPv6.h>

using namespace orion::log;
using namespace std::chrono_literals;

namespace orion
{
namespace net
{
namespace http
{
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------

AsioServerConnection::AsioServerConnection(asio::io_service& io_service,
                                           const Handlers& RequestHandlers)
   : tcp::Connection()
   , _socket(io_service)
   , _RequestHandlers(RequestHandlers)
   , _request()
   , _response(StatusCode::OK)
   , _parser()
   , _read_deadline(io_service)
   , _write_deadline(io_service)
{
}

AsioServerConnection::~AsioServerConnection()
{
   close();
}

void AsioServerConnection::close()
{
   LOG(Info) << boost::format("(%p) Connection closed") % this;

   std::error_code ec;

   _socket.close(ec);
   if (ec)
      LOG(Error) << ec;

   _read_deadline.cancel();
   _write_deadline.cancel();
}

asio::ip::tcp::socket& AsioServerConnection::socket()
{
   return _socket;
}

void AsioServerConnection::accept()
{
   _local_addr.reset(convert(_socket.local_endpoint()));
   _remote_addr.reset(convert(_socket.remote_endpoint()));

   LOG(Info) << boost::format("(%p) Connection accepted") % this;
   LOG(Info) << "   Remote address: " << _remote_addr->to_string();
   LOG(Info) << "   Local address:  " << _local_addr->to_string();

   if (default_logger().is_enabled(Level::Debug))
      dump_socket_options();

   start_read_timer();

   do_read();
}

void AsioServerConnection::start_read_timer()
{
   if (read_deadline() == std::chrono::seconds::zero())
      return;

   _read_deadline.expires_from_now(read_deadline());

   _read_deadline.async_wait(std::bind(
      &AsioServerConnection::on_read_timeout, this->shared_from_this(), std::placeholders::_1));
}

void AsioServerConnection::start_write_timer()
{
   if (write_deadline() == std::chrono::seconds::zero())
      return;

   _write_deadline.expires_from_now(write_deadline());

   _write_deadline.async_wait(std::bind(
      &AsioServerConnection::on_write_timeout, this->shared_from_this(), std::placeholders::_1));
}

void AsioServerConnection::do_read()
{
   auto self = this->shared_from_this();

   _socket.async_read_some(
      asio::buffer(_in_buffer), [this, self](std::error_code ec, std::size_t bytes_transferred) {
         if (ec)
         {
            LOG(Error) << ec;
            close();
            return;
         }

         LOG(Debug2) << "AsioServerConnection::do_read() " << int(bytes_transferred);

         ec = _parser.parse(_request, _in_buffer.data(), bytes_transferred);
         if (ec)
         {
            LOG(Error) << ec;
            close();
            return;
         }

         if (_parser.message_complete())
         {
            do_handler();
            do_write();
            return;
         }

         do_read();
      });
}

void AsioServerConnection::do_write()
{
   auto self = this->shared_from_this();

   asio::async_write(
      _socket, _response.buffers(), [this, self](std::error_code ec, std::size_t bytes_written) {
         if (ec)
         {
            LOG(Error) << ec;
            close();
            return;
         }
         std::size_t bytes_to_write = _response.size();

         LOG(Debug2) << "AsioServerConnection::do_write() " << int(bytes_to_write) << " "
                     << int(bytes_written);

         // if (bytes_to_write == bytes_written)
         //   return;

         // do_write();
      });
}

void AsioServerConnection::do_handler()
{
   LOG_FUNCTION(Debug2, "AsioServerConnection::do_handler()")

   LOG(Debug2) << _request;

   auto it = _RequestHandlers.find(_request.url().pathname());

   if (it != _RequestHandlers.end())
   {
      auto& h = it->second;

      auto ec = h(_request, _response);
      if (ec)
      {
         LOG(Error) << ec;

         _response = AsioResponse(static_cast<StatusCode>(ec.value()));
         _response.header("Connection", "close");
         _response.header("X-Content-Type-Options", "nosniff");
      }
      return;
   }
   _response = AsioResponse(StatusCode::BadRequest);
   _response.header("Connection", "close");
   _response.header("X-Content-Type-Options", "nosniff");
}

void AsioServerConnection::on_read_timeout(const asio::error_code& e)
{
   if (e == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "AsioServerConnection::on_read_timeout()")

   LOG(Debug) << e;

   close();
}

void AsioServerConnection::on_write_timeout(const asio::error_code& e)
{
   if (e == asio::error::operation_aborted)
      return;

   LOG_FUNCTION(Debug, "AsioServerConnection::on_write_timeout()")

   LOG(Debug) << e;

   close();
}

void AsioServerConnection::dump_socket_options()
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

} // http
} // net
} // orion
