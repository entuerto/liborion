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

#include <orion/net/HttpServer.h>

#include <orion/Logging.h>
#include <net/impl/MongooseHttpServer.h>


namespace orion
{
namespace net
{
using namespace orion::logging;
using namespace orion::net::mongoose;


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

std::unique_ptr<Response> HttpServer::process_request(const Request* request)
{
   LOG_FUNCTION(Debug2, "HttpServer::process_request()")

   auto it = _RequestListeners.find(request->uri());

   if (it != _RequestListeners.end())
   {
      auto&& rl = it->second;

      return rl->on_process_request(request);
   }

   return Response::create_404();
}

void HttpServer::add_request_listener(std::unique_ptr<RequestListener>&& listener)
{
   _RequestListeners.insert(std::make_pair(listener->uri(), std::move(listener)));
}

std::unique_ptr<Server> HttpServer::create(int port)
{
   return MongooseHttpServer::create(port); 
}

} // net
} // orion

