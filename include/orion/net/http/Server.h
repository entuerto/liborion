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

#include <orion/net/Server.h>
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
class API_EXPORT Server : public net::Server
{
public:
   NO_COPY(Server)

   Server();
   ~Server() override;

   Server(Server&& other) noexcept  = default;
   Server& operator=(Server&& other) noexcept  = default;

   int port() const;

   void shutdown() override;

   bool is_running() const override;

   std::error_code listen_and_serve(const std::string& addr, int port) override;

   virtual void add_handler(const std::string& p, HandlerFunc h);

private:
   const ServerImpl* impl() const { return _impl.get(); }
   ServerImpl* impl() { return _impl.get(); }
   
   std::unique_ptr<ServerImpl> _impl;
};

} // http
} // net
} // orion
#endif
