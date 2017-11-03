//
// ServerConnection.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/ServerConnection.h>

#include <orion/Log.h>

#include <boost/format.hpp>

using namespace orion::log;

namespace orion
{
namespace net
{
namespace http
{

//---------------------------------------------------------------------------------------

ServerConnection::ServerConnection(asio::io_service& io_service, const Handlers& handlers)
   : Connection(io_service)
   , _handlers(handlers)
   , _request()
   , _response(StatusCode::OK)
   , _parser()
{
}

ServerConnection::~ServerConnection()
{
}

void ServerConnection::do_read()
{
   auto self = this->shared_from_this();

   socket().async_read_some(
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
      socket(), buffers, [this, self, &buffers](std::error_code ec, std::size_t bytes_written) {
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

} // http
} // net
} // orion
