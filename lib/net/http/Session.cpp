//
// Session.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Session.h>

#include <orion/Log.h>
#include <orion/net/http/Request.h>

#include <net/http/Parser.h>

namespace orion
{
namespace net
{
namespace http
{
//--------------------------------------------------------------------------------------------------
// BaseSession

BaseSession::BaseSession(asio::io_context& io_context)
   : _io_context(io_context)
   , _socket(io_context)
   , _connected(false)
{
}

BaseSession::BaseSession(BaseSession&& rhs) noexcept
   : _io_context(rhs._io_context)
   , _socket(std::move(rhs._socket))
   , _params(std::move(rhs._params))
   , _timeout(std::move(rhs._timeout))
   , _connected(rhs._connected)
{
}

BaseSession::~BaseSession()
{
   asio::error_code ec = close();
   if (ec)
      log::error(ec);
}

BaseSession& BaseSession::operator=(BaseSession&& rhs) noexcept
{
   //_io_context = std::move(rhs._io_context);
   _socket    = std::move(rhs._socket);
   _params    = std::move(rhs._params);
   _timeout   = std::move(rhs._timeout);
   _connected = rhs._connected;

   return *this;
}

void BaseSession::set_option(const Parameters& params)
{
   _params = params;
}

void BaseSession::set_option(Parameters&& params)
{
   _params = std::move(params);
}

void BaseSession::set_option(const Timeout& timeout)
{
   _timeout = timeout;
}

bool BaseSession::connected() const
{
   return _connected;
}

void BaseSession::connected(bool value)
{
   _connected = value;
}

std::error_code BaseSession::close()
{
   asio::error_code ec;

   _connected = false;

   _socket.close(ec);
   if (ec)
      return ec;

   return std::error_code();
}

asio::io_context& BaseSession::io_context()
{
   return _io_context;
}

asio::ip::tcp::socket& BaseSession::socket()
{
   return _socket;
}

//--------------------------------------------------------------------------------------------------
// SyncSession

SyncSession::SyncSession(asio::io_context& io_context)
   : BaseSession(io_context)
{
}

SyncSession::SyncSession(SyncSession&& rhs) noexcept
   : BaseSession(std::move(rhs))
{
}

SyncSession::~SyncSession() {}

SyncSession& SyncSession::operator=(SyncSession&& rhs) noexcept
{
   BaseSession::operator=(std::move(rhs));

   return *this;
}

bool SyncSession::connect(const std::string& host, int port, std::error_code& ec)
{
   asio::ip::tcp::resolver resolver(io_context());

   auto it = resolver.resolve(host, std::to_string(port), ec);
   if (ec)
   {
      return false;
   }

   log::debug("Connecting...");

   asio::connect(socket(), it, ec);

   if (ec)
   {
      return false;
   }

   connected(true);

   socket().set_option(asio::ip::tcp::no_delay{true});
   return true;
}

Response SyncSession::submit(Request&& req)
{
   std::error_code ec;

   if (not connected())
   {
      auto url = req.url();
      if (not connect(url.hostname(), url.port(), ec))
         throw asio::system_error(ec);
   }

   // Form the request. We specify the "Connection: close" header so that the
   // server will close the socket after transmitting the response. This will
   // allow us to treat all data up until the EOF as the content.
   req.header(Field::Accept, "*/*");
   req.header(Field::Connection, "close");

   auto buffers = req.to_buffers();

   // Send the request.
   asio::write(socket(), buffers);

   /// Buffer for incoming data.
   asio::streambuf in_buffer;

   // Read the response status line. The response streambuf will automatically
   // grow to accommodate the entire line. The growth may be limited by passing
   // a maximum size to the streambuf constructor.
   while (asio::read(socket(), in_buffer, asio::transfer_all(), ec))
   {
      if (ec != asio::error::eof)
         throw asio::system_error(ec);
   }

   Response response;
   Parser parser;

   ec = parser.parse(response, in_buffer.data());
   if (ec)
   {
      throw std::system_error(ec);
   }

   return response;
}

Response SyncSession::submit(const Method& m, const Url& url)
{
   Request req{m, url};

   return submit(std::move(req));
}

//--------------------------------------------------------------------------------------------------
// AsyncSession

AsyncSession::AsyncSession(asio::io_context& io_context)
   : BaseSession(io_context)
{
}

AsyncSession::AsyncSession(AsyncSession&& rhs) noexcept
   : BaseSession(std::move(rhs))
   , _close_handler(std::move(rhs._close_handler))
   , _error_handler(std::move(rhs._error_handler))
   , _response_handler(std::move(rhs._response_handler))
   , _request(std::move(rhs._request))
   , _response(std::move(rhs._response))
{
}

AsyncSession::~AsyncSession() {}

AsyncSession& AsyncSession::operator=(AsyncSession&& rhs) noexcept
{
   BaseSession::operator=(std::move(rhs));

   _close_handler    = std::move(rhs._close_handler);
   _error_handler    = std::move(rhs._error_handler);
   _response_handler = std::move(rhs._response_handler);

   _request  = std::move(rhs._request);
   _response = std::move(rhs._response);

   return *this;
}

void AsyncSession::on_close(CloseHandler h)
{
   _close_handler = std::move(h);
}

void AsyncSession::on_error(ErrorHandler h)
{
   _error_handler = std::move(h);
}

void AsyncSession::on_response(ResponseHandler h)
{
   _response_handler = std::move(h);
}

void AsyncSession::connect(const std::string& host, int port)
{
   asio::ip::tcp::resolver resolver(io_context());

   std::error_code ec;

   auto it = resolver.resolve(host, std::to_string(port), ec);
   if (ec)
   {
      do_on_error(ec);
      return;
   }

   log::debug("Connecting...");

   auto self = shared_from_this();

   asio::async_connect(
      socket(), it, [self](const std::error_code& ec, const asio::ip::tcp::endpoint&) {
         if (ec)
         {
            self->do_on_error(ec);
            return;
         }
         self->connected(true);
         self->socket().set_option(asio::ip::tcp::no_delay{true});

         log::info("Connected to host:");
         log::info("   Remote address: ", self->socket().remote_endpoint());
         log::info("   Local address:  ", self->socket().local_endpoint());

         self->do_write();
      });
}

void AsyncSession::submit(Request&& req)
{
   _request = std::move(req);

   // Form the request. We specify the "Connection: close" header so that the
   // server will close the socket after transmitting the response. This will
   // allow us to treat all data up until the EOF as the content.
   _request.header(Field::Accept, "*/*");
   _request.header(Field::Connection, "close");

   if (not connected())
   {
      auto url = _request.url();
      connect(url.hostname(), url.port());
   }
}

void AsyncSession::submit(const Method& m, const Url& url)
{
   Request req{m, url};

   submit(std::move(req));
}

void AsyncSession::do_read()
{
   if (not connected())
      return;

   log::debug("Reading...");

   auto self = shared_from_this();

   socket().async_read_some(_in_buffer.prepare(8192),
      [self](std::error_code ec, std::size_t bytes_transferred) {
         if (ec)
         {
            log::error(ec, _src_loc);
            self->close();
            return;
         }

         self->_in_buffer.commit(bytes_transferred);

         log::debug2("Read bytes ", int(bytes_transferred));

         Parser parser;
         ec = parser.parse(self->_response, self->_in_buffer.data());
         if (ec)
         {
            log::error(ec, _src_loc);
            self->close();
            return;
         }

         if (parser.message_complete())
         {
            self->do_on_response(self->_response);
            return;
         }
         self->do_read();
      });
}

void AsyncSession::do_write()
{
   if (not connected())
      return;

   log::debug("Sending request...");

   auto self = shared_from_this();

   auto out_buffers    = _request.to_buffers();
   auto bytes_to_write = asio::buffer_size(out_buffers);

   asio::async_write(
      socket(),
      out_buffers,
      [self, bytes_to_write](const std::error_code& ec, std::size_t bytes_written) {
         if (ec)
         {
            log::error(ec, _src_loc);
            self->close();
            return;
         }

         log::debug2("Sent bytes ", int(bytes_to_write), " ", int(bytes_written));

         self->do_read();
      });
}

void AsyncSession::do_on_close()
{
   if (_close_handler)
      _close_handler();
}

void AsyncSession::do_on_error(const std::error_code& ec)
{
   if (_error_handler)
      _error_handler(ec);
}

void AsyncSession::do_on_response(const Response& res)
{
   if (_response_handler)
      _response_handler(res);
}

} // namespace http
} // namespace net
} // namespace orion
