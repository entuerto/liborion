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

int Server::port() const
{
   return impl()->port();
}

void Server::add_handler(const std::string& p, HandlerFunc h)
{
   impl()->add_handler(p, std::move(h));
}

void Server::shutdown()
{
   impl()->shutdown();
}

bool Server::is_running() const
{
   return impl()->is_running();
}

std::error_code Server::listen_and_serve(const net::EndPoint& ep)
{
   auto addr = asio::ip::make_address(to_string(ep.address()));
   asio::ip::tcp::endpoint endpoint{addr, ep.port()};

   return impl()->listen_and_serve(std::move(endpoint));
}

} // http
} // net
} // orion
