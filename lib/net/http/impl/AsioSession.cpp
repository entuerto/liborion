//
// AsioSession.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/http/Session.h>

#include <iostream>

#include <net/http/impl/AsioRequest.h>
#include <net/http/impl/AsioResponse.h>
#include <net/http/impl/Parser.h>

#include <asio.hpp>

namespace orion
{
namespace net
{
namespace http
{

Response Session::operator()(const Method& m)
{
   asio::io_service io_service;

   auto req = AsioRequest(m, _url, Version{1, 1}, _header);

   // Get a list of endpoints corresponding to the server name.
   asio::ip::tcp::resolver resolver(io_service);
   asio::ip::tcp::resolver::query query(_url.hostname(), std::to_string(_url.port()));

   auto endpoint_iterator = resolver.resolve(query);

   // Try each endpoint until we successfully establish a connection.
   asio::ip::tcp::socket socket(io_service);
   asio::connect(socket, endpoint_iterator);
   
   // Form the request. We specify the "Connection: close" header so that the
   // server will close the socket after transmitting the response. This will
   // allow us to treat all data up until the EOF as the content.
   req.header("Accept", "*/*");
   req.header("Connection", "close");

   // Send the request.
   asio::write(socket, req.buffers());

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
   AsioResponse response(StatusCode::OK);

   ec = parser.parse(response, asio::buffer_cast<const char*>(response_buffer.data()), response_buffer.size()); 
   if (ec)
   {
      throw std::system_error(ec);
   }  

   return std::move(response); 
}

} // http
} // net
} // orion
