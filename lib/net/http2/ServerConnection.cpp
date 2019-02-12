//
// ServerConnection.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include "ServerConnection.h"

#include "Handler.h"

#include <orion/Log.h>

namespace orion
{
namespace net
{
namespace http2
{

//---------------------------------------------------------------------------------------

ServerConnection::ServerConnection(asio::ip::tcp::socket socket, http::RequestMux& mux)
   : Connection(std::move(socket))
   , _mux(mux)
{
}

void ServerConnection::do_accept()
{
   _handler = std::make_shared<Handler>(socket().get_executor().context(), _mux);

   // TODO: start reading or writing?
}

void ServerConnection::do_read()
{
   log::debug2("Reading...");

   auto self = this->shared_from_this();

   start_read_timer();

   socket().async_read_some(
      asio::buffer(_in_buffer), [this, self](std::error_code ec, std::size_t bytes_transferred) {
         if (ec)
         {
            log::error(ec, DbgSrcLoc);
            close();
            return;
         }

         log::debug2("Read - Bytes transferred: ", int(bytes_transferred));

         ec = _handler->on_read(asio::buffer(_in_buffer), bytes_transferred);
         if (ec)
         {
            log::error(ec, DbgSrcLoc);
            close();
            return;
         }

         do_write();

         if (not _writing and _handler->should_stop())
         {
            close();
            return;
         }
         
         do_read();
         
      });
}

void ServerConnection::do_write()
{
   if (_writing)
      return;

   std::size_t bytes_to_write{0};

   asio::mutable_buffer b = asio::buffer(_out_buffer);

   std::error_code ec = _handler->on_write(b, bytes_to_write);
   if (ec)
   {
      log::error(ec, DbgSrcLoc);
      close();
      return;
   }

   if (bytes_to_write == 0)
   {
      if (_handler->should_stop())
         close();
      return;
   }

   log::debug2("Writting...");

   _writing = true;

   // Reset read deadline here, because normally client is sending
   // something, it does not expect timeout while doing it.
   start_read_timer();

   auto self = this->shared_from_this();

   asio::async_write(
      socket(),
      asio::buffer(_out_buffer, bytes_to_write),
      [this, self, bytes_to_write](std::error_code ec, std::size_t bytes_written) {
         if (ec)
         {
            log::error(ec, DbgSrcLoc);
            close();
            return;
         }
         
         _writing = false;

         log::debug2("Write - Bytes written ", int(bytes_written), "/", int(bytes_to_write));

         do_write();
      });
}

} // namespace http2
} // namespace net
} // namespace orion
