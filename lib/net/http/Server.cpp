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

uint16_t Server::port() const
{
   return impl()->port();
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
