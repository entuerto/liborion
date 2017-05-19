//
// Server.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/http/Server.h>

#include <net/http/impl/AsioServer.h>

namespace orion
{
namespace net
{
namespace http
{
Server::Server()
   : _port(-1)
   , _is_running(false)
   , _RequestHandlers()
{
}

Server::~Server()
{
}

int Server::port() const
{
   return _port;
}

void Server::add_handler(std::unique_ptr<RequestHandler>&& h)
{
   _RequestHandlers.insert(std::make_pair(h->uri(), std::move(h)));
}

bool Server::is_running() const
{
   return _is_running;
}

std::unique_ptr<http::Server> Server::create()
{
   return std::make_unique<AsioServer>();
}

} // http
} // net
} // orion
