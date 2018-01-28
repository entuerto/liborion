//
// Server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_TCP_SERVER_H
#define ORION_NET_TCP_SERVER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/Server.h>
#include <orion/net/tcp/Utils.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{
namespace tcp
{
class ServerImpl;

/// This class provides a tcp protocol Server
///
/// This class provides a tcp protocol Server to handle incoming requests
/// and sends valid responses.
///
class API_EXPORT Server : public net::Server
{
public:
   NO_COPY(Server)

   Server();
   Server(Handler h);
   ~Server() override;

   Server(Server&& other) noexcept  = default;
   Server& operator=(Server&& other) noexcept  = default;

   int port() const;

   void register_handler(Handler h);

   void shutdown() override;

   bool is_running() const override;

   std::error_code listen_and_serve(const std::string& addr, int port) override;

private:
   const ServerImpl* impl() const { return _impl.get(); }
   ServerImpl* impl() { return _impl.get(); }

   std::unique_ptr<ServerImpl> _impl;
};

} // tcp
} // net
} // orion
#endif // ORION_NET_TCP_SERVER_H
