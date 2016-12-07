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

#include <net/impl/MongooseHttpServer.h>

#include <cstring>
#include <iostream>
#include <utility>

#include <boost/format.hpp>

#include <orion/Logging.h>
#include <orion/StringUtils.h>
#include <net/impl/MongooseRequest.h>

namespace orion
{
namespace net
{
namespace mongoose
{
using namespace orion::logging;

static void request_handler(struct mg_connection* nc, int event, void* ev_data)
{
   struct mg_mgr* mgr = nc->mgr;
   struct http_message* hm = static_cast<struct http_message*>(ev_data);

   switch (event)
   {
      case MG_EV_ACCEPT:
      {
         char addr[32];
         mg_conn_addr_to_str(nc, addr, sizeof(addr), MG_SOCK_STRINGIFY_REMOTE |
                                                     MG_SOCK_STRINGIFY_IP     |
                                                     MG_SOCK_STRINGIFY_PORT);

         LOG(Info) << boost::format("Accepting connection %p from %s") % nc % addr;
         break;
      }
      case MG_EV_HTTP_REQUEST:
      {
         MongooseHttpServer* server = static_cast<MongooseHttpServer*>(mgr->user_data);

         std::unique_ptr<Request>  request = MongooseRequest::create(nc, hm);
         std::unique_ptr<Response> response = server->process_request(request.get());

         server->send_response(response.get(), nc);
         break;
      }
      case MG_EV_CLOSE:
      {
         LOG(Info) << boost::format("Closed connection %p") % nc;
         break;
      }
      default:
         break;
   }
}

MongooseHttpServer::MongooseHttpServer(int port) :
   HttpServer(port),
   _is_running(false),
   _mgr()
{
   mg_mgr_init(&_mgr, this);
}

MongooseHttpServer::~MongooseHttpServer()
{
   stop();

   mg_mgr_free(&_mgr);
}

void MongooseHttpServer::start()
{
   LOG(Debug) << "listening port: " 
              << _port;

   struct mg_connection* nc = mg_bind(&_mgr, to_string(port()).c_str(), request_handler);

   mg_set_protocol_http_websocket(nc);

   _is_running = true;

   while (is_running()) 
   {
      mg_mgr_poll(&_mgr, 1000);
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

   mg_send(connection, buffer.c_str(), buffer.size());
}

std::unique_ptr<Server> MongooseHttpServer::create(int port)
{
   return std::make_unique<MongooseHttpServer>(port); 
}

} // mongoose
} // net
} // orion

