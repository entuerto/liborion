//
// ServerConnection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/ServerConnection.h>

#include <orion/Log.h>
#include <orion/net/AddressV4.h>
#include <orion/net/AddressV6.h>

#include <boost/format.hpp>

using namespace orion::log;
using namespace std::chrono_literals;

namespace orion
{
namespace net
{
namespace http
{
//---------------------------------------------------------------------------------------
EndPoint* convert(const asio::ip::tcp::endpoint& ep)
{
   auto addr = ep.address();

   if (addr.is_v4())
   {
      auto addr_v4 = addr.to_v4();
      return new EndPoint(AddressV4(addr_v4.to_bytes()), ep.port());
   }

   if (addr.is_v6())
   {
      auto addr_v6 = addr.to_v6();
      return new EndPoint(AddressV6(addr_v6.to_bytes()), ep.port());
   }

   return nullptr;
}

//---------------------------------------------------------------------------------------

ServerConnection::ServerConnection(asio::io_service& io_service, const Handlers& handlers)
   : tcp::Connection()
   , _socket(io_service)
   , _handlers(handlers)
   , _request()
   , _response(StatusCode::OK)
   , _parser()
   , _read_deadline(io_service)
   , _write_deadline(io_service)
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
      LOG(Error) << ec;

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

std::error_code ServerConnection::on_handler(std::streambuf*, std::streambuf*)
{
   return std::error_code();
}

asio::ip::tcp::socket& ServerConnection::socket()
{
   return _socket;
}

void ServerConnection::accept()
{
   _local_endpoint.reset(convert(_socket.local_endpoint()));
   _remote_endpoint.reset(convert(_socket.remote_endpoint()));

   LOG(Info) << boost::format("(%p) Connection accepted") % this;
   LOG(Info) << "   Remote address: " << _remote_endpoint->to_string();
   LOG(Info) << "   Local address:  " << _local_endpoint->to_string();

   if (default_logger().is_enabled(Level::Debug))
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
      asio::buffer(_in_buffer), [this, self](std::error_code ec, std::size_t bytes_transferred) {
         if (ec)
         {
            LOG(Error) << ec;
            close();
            return;
         }

         LOG(Debug2) << "ServerConnection::do_read() " << int(bytes_transferred);

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

void ServerConnection::do_write()
{
   auto self = this->shared_from_this();

   std::vector<asio::const_buffer> buffers;
   buffers.push_back(static_cast<asio::streambuf*>(_response.header_rdbuf())->data());
   buffers.push_back(static_cast<asio::streambuf*>(_response.body_rdbuf())->data());

   asio::async_write(
      _socket, buffers, [this, self, &buffers](std::error_code ec, std::size_t bytes_written) {
         if (ec)
         {
            LOG(Error) << ec;
            close();
            return;
         }
         std::size_t bytes_to_write = asio::buffer_size(buffers);

         LOG(Debug2) << "ServerConnection::do_write() " << int(bytes_to_write) << " "
                     << int(bytes_written);

         // if (bytes_to_write == bytes_written)
         //   return;

         // do_write();
      });
}

void ServerConnection::do_handler()
{
   LOG_FUNCTION(Debug2, "ServerConnection::do_handler()")

   LOG(Debug2) << _request;

   auto it = _handlers.find(_request.url().pathname());

   if (it != _handlers.end())
   {
      auto& h = it->second;

      auto ec = h(_request, _response);
      if (ec)
      {
         LOG(Error) << ec;

         _response = Response(static_cast<StatusCode>(ec.value()));
         _response.header("Connection", "close");
         _response.header("X-Content-Type-Options", "nosniff");
      }
      return;
   }
   _response = Response(StatusCode::BadRequest);
   _response.header("Connection", "close");
   _response.header("X-Content-Type-Options", "nosniff");
}

void ServerConnection::on_read_timeout(const asio::error_code& e)
{
   if (e == asio::error::operation_aborted)
      return;

   log::warning("ServerConnection: Read timeout");

   LOG(Debug) << e;

   close();
}

void ServerConnection::on_write_timeout(const asio::error_code& e)
{
   if (e == asio::error::operation_aborted)
      return;

   log::warning("ServerConnection: Write timeout");

   LOG(Debug) << e;

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

} // http
} // net
} // orion
