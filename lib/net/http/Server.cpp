//
// Server.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
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

void Server::add_handler(const std::string& p, HandlerFunc h)
{
   _impl->add_handler(p, std::move(h));
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

} // http
} // net
} // orion
