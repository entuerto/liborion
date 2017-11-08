//
// Server.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/tcp/Server.h>

#include <net/tcp/ServerImpl.h>

namespace orion
{
namespace net
{
namespace tcp
{

Server::Server()
   : _impl(std::make_unique<ServerImpl>())
{
}

Server::Server(Handler h)
   : _impl(std::make_unique<ServerImpl>(std::move(h)))
{
}

Server::Server(Server&& other) noexcept
   : _impl(std::move(other._impl))
{
}  

Server::~Server()
{
}

Server& Server::operator=(Server&& other) noexcept
{
   _impl = std::move(other._impl);

   return *this;
}

int Server::port() const
{
   return _impl->port();
}

void Server::register_handler(Handler h)
{
   _impl->register_handler(std::move(h));
}

void Server::shutdown()
{
   _impl->shutdown();
}

bool Server::is_running() const
{
   return _impl->is_running();
}

std::error_code Server::listen_and_serve(const std::string& addr, int port)
{
   return _impl->listen_and_serve(addr, port);
}

} // tcp
} // net
} // orion