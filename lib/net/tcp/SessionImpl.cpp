//
// SessionImpl.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/tcp/SessionImpl.h>

#include <orion/Log.h>

#include <iostream>

namespace orion
{
namespace net
{
namespace tcp
{

SessionImpl::SessionImpl(asio::io_context& io_context)
   : _params()
   , _timeout()
   , _connected(false)
   , _connect_handler()
   , _read_handler()
   , _write_handler()
   , _io_context(io_context)
   , _socket(io_context)
   , _in_streambuf()
   , _out_streambuf()
{
}

SessionImpl::~SessionImpl()
{
   asio::error_code ec = close();
   if (ec)
      log::error(ec);
}

void SessionImpl::set_option(const Parameters& params)
{
   _params = params;
}

void SessionImpl::set_option(Parameters&& params)
{
   _params = std::move(params);
}

void SessionImpl::set_option(const Timeout& timeout)
{
   _timeout = timeout;
}

bool SessionImpl::connected() const
{
   return _connected;
}

void SessionImpl::on_connect(ConnectHandler h)
{
   _connect_handler = std::move(h);
}

void SessionImpl::on_read(ReadHandler h)
{
   _read_handler = std::move(h);
}

void SessionImpl::on_write(WriteHandler h)
{
   _write_handler = std::move(h);
}

void SessionImpl::connect(EndPoint endpoint)
{
   auto addr = asio::ip::make_address(to_string(endpoint.address()));

   asio::ip::tcp::endpoint ep{addr, endpoint.port()};

   asio::ip::tcp::resolver resolver(_io_context);

   std::error_code ec;

   auto it = resolver.resolve(ep, ec);
   if (ec)
   {
      _connect_handler(ec);
      return;
   }

   log::debug("Connecting...");

   auto self = shared_from_this();

   asio::async_connect(_socket, it,
      [self](const std::error_code& ec, const asio::ip::tcp::endpoint& /* endpoint */)
      {
         if (not ec)
            self->_connected = true;

         self->_connect_handler(ec);

         self->do_read();
      });
}

void SessionImpl::write(std::streambuf* streambuf)
{
   if (not _connected)
      return;

   log::debug("Sending...");

   std::ostream out(&_out_streambuf);

   out << streambuf;

   do_write();
}

void SessionImpl::write(const uint8_t* data, std::size_t len)
{
   if (not _connected)
      return;

   log::debug("Sending...");

   std::ostream out(&_out_streambuf);

   out.write(reinterpret_cast<const char*>(data), len);

   do_write();
}

std::error_code SessionImpl::close()
{
   asio::error_code ec;
   
   _connected = false;

   _socket.close(ec);
   if (ec)
      return ec;

   return std::error_code();
}

void SessionImpl::do_read()
{
   if (not _connected)
      return;

   log::debug("Reading...");

   auto self = shared_from_this();

   _socket.async_read_some(_in_streambuf.prepare(1024),
      [self](const std::error_code& ec, std::size_t bytes_transferred)
      {
         log::debug("Bytes read: ", bytes_transferred);

         self->_in_streambuf.commit(bytes_transferred);

         self->_read_handler(ec, self->_in_streambuf);

         self->do_read();
      });
}

void SessionImpl::do_write()
{
   if (not _connected)
      return;

   log::debug("Writting...");

   auto self = shared_from_this();

   asio::async_write(_socket, _out_streambuf, 
      [self](const std::error_code& ec, std::size_t bytes_written)
      {
         self->_write_handler(ec, bytes_written);

         if (bytes_written != 0)
            self->do_write();
      });
}

} // tcp
} // net
} // orion
