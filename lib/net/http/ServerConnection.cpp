//
// ServerConnection.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/http/ServerConnection.h>

#include <orion/Log.h>

using namespace orion::log;

namespace orion
{
namespace net
{
namespace http
{

//---------------------------------------------------------------------------------------

ServerConnection::ServerConnection(asio::ip::tcp::socket socket, RequestMux& mux)
   : Connection(std::move(socket))
   , _mux(mux)
   , _request()
   , _response(StatusCode::OK)
   , _parser()
{
}

ServerConnection::~ServerConnection() 
{
   log::debug2("~ServerConnection()");
}

void ServerConnection::do_read()
{
   auto self = this->shared_from_this();

   socket().async_read_some(
      asio::buffer(_in_buffer), [this, self](std::error_code ec, std::size_t bytes_transferred) {
         if (ec)
         {
            log::error(ec, _src_loc);
            close();
            return;
         }

         log::debug2("Connection::do_read() ", int(bytes_transferred));

         ec = _parser.parse(_request, asio::const_buffer(_in_buffer.data(), bytes_transferred));
         if (ec)
         {
            log::error(ec, _src_loc);
            close();
            return;
         }
         log::debug2("After parse");

         if (_parser.message_complete())
         {
            do_handler();
            do_write();
            return;
         }
         log::debug2("Before do_read()");
         do_read();
      });
}

void ServerConnection::do_write()
{
   auto self = this->shared_from_this();

   auto buffers = _response.to_buffers();

   std::size_t bytes_to_write = asio::buffer_size(buffers);

   asio::async_write(
      socket(), buffers, [this, self, bytes_to_write](std::error_code ec, std::size_t bytes_written) {
         if (ec)
         {
            log::error(ec, _src_loc);
            close();
            return;
         }

         log::debug2("Connection::do_write() ", int(bytes_to_write), " ", int(bytes_written));

         close();
      });
}

void ServerConnection::do_handler()
{
   LOG_FUNCTION(Debug2, "ServerConnection::do_handler()")

   log::debug2(_request);

   _mux(_request, _response);

   log::debug2(_response);
}

} // http
} // net
} // orion
