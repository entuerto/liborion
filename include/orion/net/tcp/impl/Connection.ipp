//
// Connection.ipp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_TCP_CONNECTION_IPP
#define ORION_TCP_CONNECTION_IPP

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace tcp
{

inline std::error_code set_option(Connection& conn, const NoDelay& value)
{
   asio::error_code ec;
   conn.socket().set_option(asio::ip::tcp::no_delay(value), ec);
   return ec;
}

//--------------------------------------------------------------------------------------------------

Connection::Connection(asio::ip::tcp::socket socket, const Handler& handler)
   : net::Connection<asio::ip::tcp::socket>(std::move(socket))
   , _handler(handler)
   , _in_streambuf()
   , _out_streambuf()
   , _in_buffer_size(8192)
{
}

Connection::~Connection() = default;

void Connection::do_read()
{
   auto self = this->shared_from_this();

   auto b = _in_streambuf.prepare(_in_buffer_size);

   socket().async_read_some(b, [this, self](std::error_code ec, std::size_t bytes_transferred) {
      if (ec)
      {
         log::error(ec, _src_loc);
         close();
         return;
      }
      log::debug2("Connection::do_read() ", int(bytes_transferred));

      _in_streambuf.commit(bytes_transferred);

      ec = _handler(_in_streambuf, _out_streambuf);
      if (ec)
      {
         log::error(ec, _src_loc);
         close();
         return;
      }
      do_write();

      do_read();
   });
}

void Connection::do_write()
{
   auto self = this->shared_from_this();

   asio::async_write(
      socket(), _out_streambuf.data(), [this, self](std::error_code ec, std::size_t bytes_written) {
         if (ec)
         {
            log::error(ec, _src_loc);
            close();
            return;
         }
         std::size_t bytes_to_write = _out_streambuf.size();

         _out_streambuf.consume(bytes_written);

         log::debug2("Connection::do_write() ", int(bytes_to_write), " ", int(bytes_written));

         if (bytes_to_write == bytes_written)
            return;

         do_write();
      });
}

} // tcp
} // net
} // orion
#endif // ORION_TCP_CONNECTION_IPP