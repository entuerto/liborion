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

#include <ws/mongoose/MongooseHttpServer.h>

#include <cstring>
#include <iostream>
#include <utility>

#include <orion/Logging.h>
#include <orion/StringUtils.h>
#include <ws/mongoose/MongooseRequest.h>

namespace orion
{
namespace ws
{
namespace mongoose
{
using namespace orion::logging;

static int begin_request(struct mg_connection* connection)
{
   const struct mg_request_info* request_info = mg_get_request_info(connection);

   MongooseHttpServer* server = static_cast<MongooseHttpServer*>(request_info->user_data);

   Response::SharedPtr response = server->process_request(MongooseRequest::create(connection));

   server->send_response(response, connection);

   return 1; //  processed
}

MongooseHttpServer::MongooseHttpServer() :
   HttpServer(),
   _is_running(false),
   _ctx(nullptr)
{
}

MongooseHttpServer::~MongooseHttpServer()
{
   stop();
}

void MongooseHttpServer::start()
{
   std::string ports = make_port_list();

   LOG(Debug) << "listening ports: " 
              << ports;

   const char *options[] = { "listening_ports", ports.c_str(), NULL };

   struct mg_callbacks callbacks;
   memset(&callbacks, 0, sizeof(callbacks));

   callbacks.begin_request = begin_request;

   _ctx = mg_start(&callbacks, this, options);

   if (_ctx != nullptr)
      _is_running = true;
}

void MongooseHttpServer::stop()
{
   if (not _is_running)
      return;

   mg_stop(_ctx);

   _is_running = false;
}

bool MongooseHttpServer::is_running() const
{
   return _is_running;
}

void MongooseHttpServer::send_response(Response::SharedPtr response)
{
   
}

void MongooseHttpServer::send_response(Response::SharedPtr response, struct mg_connection* connection)
{
   std::string buffer = response->to_string();

   LOG(Debug) << buffer;

   mg_write(connection, buffer.c_str(), buffer.size());
}

std::string MongooseHttpServer::make_port_list()
{
   auto it = _RequestListeners.begin();

   std::string ports = std::to_string(it->first);
   for (it++; it != _RequestListeners.end(); ++it) 
   {
      ports += ',';
      ports += std::to_string(it->first);
   }
   return ports;
}

Server::SharedPtr MongooseHttpServer::create()
{
   return Server::SharedPtr(new MongooseHttpServer); 
}

} // mongoose
} // ws
} // orion

