//
// Server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_SERVER_H
#define ORION_NET_HTTP_SERVER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/EndPoint.h>
#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Utils.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{
namespace http
{
class ServerImpl;

/// This class provides an embedded HTTP Server
///
/// This class provides an embedded HTTP Server to handle incoming Requests
/// and send HTTP 1.1 valid responses.
///
class API_EXPORT Server 
{
public:
   NO_COPY(Server)

   Server();
   virtual ~Server();

   DEFAULT_MOVE(Server)

   EndPoint endpoint() const;

   void shutdown();

   bool is_running() const;

   RequestMux& request_mux();

   std::error_code listen_and_serve(EndPoint endpoint);

   std::error_code listen_and_serve(EndPoint endpoint, RequestMux mux);

private:
   const ServerImpl* impl() const { return _impl.get(); }
   ServerImpl* impl() { return _impl.get(); }
   
   std::unique_ptr<ServerImpl> _impl;
};

} // http
} // net
} // orion
#endif
