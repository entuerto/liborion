//
// Session.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Session.h>

#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <net/http/Parser.h>

#include <asio.hpp>

#include <iostream>
#include <utility>

namespace orion
{
namespace net
{
namespace http
{

Session::Session()
   : _url()
   , _params()
   , _header()
   , _timeout()
{
}

Session::Session(Session&& rhs)
   : _url(std::move(rhs._url))
   , _params(std::move(rhs._params))
   , _header(std::move(rhs._header))
   , _timeout(std::move(rhs._timeout))
{
}

Session::~Session()
{
}

Session& Session::operator=(Session&& rhs)
{
   _url     = std::move(rhs._url);
   _params  = std::move(rhs._params);
   _header  = std::move(rhs._header);
   _timeout = std::move(rhs._timeout);
   return *this;
}

void Session::set_option(const Url& url)
{
   _url = url;
}

void Session::set_option(const Parameters& params)
{
   _params = params;
}

void Session::set_option(Parameters&& params)
{
   _params = std::move(params);
}

void Session::set_option(const Header& header)
{
   _header = header;
}

void Session::set_option(const Timeout& timeout)
{
   _timeout = timeout;
}

Response Session::operator()(const Method& m)
{
   asio::io_context io_context;

   auto req = Request(m, _url, Version{1, 1}, _header);

   // Get a list of endpoints corresponding to the server name.
   asio::ip::tcp::resolver resolver(io_context);
   asio::ip::tcp::resolver::query query(_url.hostname(), std::to_string(_url.port()));

   auto endpoint_iterator = resolver.resolve(query);

   // Try each endpoint until we successfully establish a connection.
   asio::ip::tcp::socket socket(io_context);
   asio::connect(socket, endpoint_iterator);

   // Form the request. We specify the "Connection: close" header so that the
   // server will close the socket after transmitting the response. This will
   // allow us to treat all data up until the EOF as the content.
   req.header("Accept", "*/*");
   req.header("Connection", "close");

   auto buffers = req.to_buffers();

   // Send the request.
   asio::write(socket, buffers);

   // Read the response status line. The response streambuf will automatically
   // grow to accommodate the entire line. The growth may be limited by passing
   // a maximum size to the streambuf constructor.
   asio::streambuf response_buffer;

   std::error_code ec;

   while (asio::read(socket, response_buffer, asio::transfer_all(), ec))
   {
      if (ec != asio::error::eof)
         throw asio::system_error(ec);
   }

   Parser parser;
   Response response(StatusCode::OK);

   ec = parser.parse(
      response, asio::buffer_cast<const char*>(response_buffer.data()), response_buffer.size());
   if (ec)
   {
      throw std::system_error(ec);
   }

   return response;
}

} // http
} // net
} // orion
