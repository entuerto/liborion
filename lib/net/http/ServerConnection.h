//
// ServerConnection.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_SERVERCONNECTION_H
#define ORION_NET_HTTP_SERVERCONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Connection.h>
#include <orion/net/http/Request.h>
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
class ServerConnection 
   : public Connection<TcpSocket>
{
public:
   ServerConnection(asio::io_service& io_service, const Handlers& handlers);
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
   const Handlers& _handlers;

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
