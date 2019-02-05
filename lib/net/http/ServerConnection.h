//
// ServerConnection.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_SERVERCONNECTION_H
#define ORION_NET_HTTP_SERVERCONNECTION_H

#include <orion/Common.h>

#include <orion/net/Connection.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>

#include <net/http/Parser.h>

#include <asio.hpp>

#include <array>
#include <memory>

namespace orion
{
namespace net
{
namespace http
{

///
class ServerConnection : public Connection<asio::ip::tcp::socket>
{
public:
   ServerConnection(asio::ip::tcp::socket socket, RequestMux& mux);
   virtual ~ServerConnection();

protected:
   /// Perform an asynchronous read operation.
   void do_read() override;

   /// Perform an asynchronous write operation.
   void do_write() override;

private:
   /// Process the request.
   void do_handler();

   /// Request handlers
   RequestMux& _mux;

   Request _request;
   Response _response;

   /// Buffer for incoming data. 8k
   std::array<char, 8192> _in_buffer;

   Parser _parser;
};

} // http
} // net
} // orion
#endif // ORION_NET_HTTP_SERVERCONNECTION_H
