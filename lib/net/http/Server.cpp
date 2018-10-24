//
// Server.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Server.h>

#include <net/http/ServerImpl.h>

namespace orion
{
namespace net
{
namespace http
{
Server::Server()
   : _impl(std::make_unique<ServerImpl>())
{
}

Server::~Server() = default;

EndPoint Server::endpoint() const
{
   return impl()->endpoint();
}

void Server::shutdown()
{
   impl()->shutdown();
}

bool Server::is_running() const
{
   return impl()->is_running();
}

RequestMux& Server::request_mux()
{
   return impl()->request_mux();
}

void Server::tls_handshake_timeout(const std::chrono::seconds& t)
{
   return impl()->tls_handshake_timeout(t);
}

void Server::read_timeout(const std::chrono::seconds& t)
{
   return impl()->read_timeout(t);
}

std::error_code Server::listen_and_serve(EndPoint endpoint)
{
   return impl()->listen_and_serve(std::move(endpoint));
}

std::error_code Server::listen_and_serve(EndPoint endpoint, RequestMux mux)
{
   return impl()->listen_and_serve(std::move(endpoint), std::move(mux));
}

} // http
} // net
} // orion
