/*
 * HttpServer.cpp
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <orion/ws/HttpServer.h>

#include <orion/Logging.h>
#include <orion/ws/InetAddress.h>
#include <ws/impl/MongooseHttpServer.h>


namespace orion
{
namespace ws
{
using namespace orion::logging;
using namespace orion::ws::mongoose;


HttpServer::HttpServer(int port) :
   _port(port),
   _RequestListeners()
{
}

HttpServer::~HttpServer()
{
}

int HttpServer::port() const
{
   return _port;
}

Response::SharedPtr HttpServer::process_request(Request::SharedPtr request)
{
   LOG_FUNCTION(Debug2, "HttpServer::process_request()")

   auto it = _RequestListeners.find(request->uri());

   if (it != _RequestListeners.end())
   {
      RequestListener::SharedPtr rl = it->second;

      return rl->on_process_request(request);
   }

   return Response::create_404();
}

void HttpServer::add_request_listener(RequestListener::SharedPtr listener)
{
   _RequestListeners.insert(std::pair<std::string, RequestListener::SharedPtr>(listener->uri(), listener));
}

Server::SharedPtr HttpServer::create(int port)
{
   return MongooseHttpServer::create(port); 
}

} // ws
} // orion

