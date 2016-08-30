/*
 * MongooseHttpServer.cpp
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

#include <ws/impl/MongooseHttpServer.h>

#include <cstring>
#include <iostream>
#include <utility>

#include <orion/Logging.h>
#include <orion/StringUtils.h>
#include <ws/impl/MongooseRequest.h>

namespace orion
{
namespace ws
{
namespace mongoose
{
using namespace orion::logging;

static int request_handler(struct mg_connection* connection, enum mg_event event)
{
   int result = MG_FALSE;

   switch (event)
   {
      case MG_REQUEST:
      {
         MongooseHttpServer* server = static_cast<MongooseHttpServer*>(connection->server_param);

         std::unique_ptr<Request>  request = MongooseRequest::create(connection);
         std::unique_ptr<Response> response = server->process_request(request.get());

         server->send_response(response.get(), connection);

         result = MG_TRUE;
         break;
      }
      case MG_AUTH:
      {
         result = MG_TRUE;
         break;
      }
   }

   return result; //  processed
}

MongooseHttpServer::MongooseHttpServer(int port) :
   HttpServer(port),
   _is_running(false),
   _server(nullptr)
{
   _server = mg_create_server(this, request_handler);
}

MongooseHttpServer::~MongooseHttpServer()
{
   stop();

   mg_destroy_server(&_server);
}

void MongooseHttpServer::start()
{
   LOG(Debug) << "listening port: " 
              << _port;

   mg_set_option(_server, "listening_port", to_string(port()).c_str());

   _is_running = true;

   while (is_running()) 
   {
      mg_poll_server(_server, 1000);
   }
   
}

void MongooseHttpServer::stop()
{
   if (not _is_running)
      return;

   _is_running = false;
}

bool MongooseHttpServer::is_running() const
{
   return _is_running;
}

void MongooseHttpServer::send_response(const Response* /* response */)
{
   
}

void MongooseHttpServer::send_response(const Response* response, struct mg_connection* connection)
{
   std::string buffer = response->to_string();

   LOG(Debug) << buffer;

   mg_write(connection, buffer.c_str(), buffer.size());
}

std::unique_ptr<Server> MongooseHttpServer::create(int port)
{
   return std::make_unique<MongooseHttpServer>(port); 
}

} // mongoose
} // ws
} // orion

