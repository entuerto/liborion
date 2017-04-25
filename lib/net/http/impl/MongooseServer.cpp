/*
 * MongooseServer.cpp
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

#include <net/http/impl/MongooseServer.h>

#include <cstring>
#include <iostream>
#include <utility>

#include <orion/Logging.h>

#include <net/http/impl/MongooseServerConnection.h>

#include <boost/format.hpp>

using namespace orion::logging;

namespace orion
{
namespace net
{
namespace http
{

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
         MongooseServer* server = static_cast<MongooseServer*>(mgr->user_data);

         server->serve(nc, hm);

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

MongooseServer::MongooseServer() :
   Server(),
   _mgr()
{
   mg_mgr_init(&_mgr, this);
}

MongooseServer::~MongooseServer()
{
   mg_mgr_free(&_mgr);
}

void MongooseServer::shutdown()
{
}

std::error_code MongooseServer::listen_and_serve(const std::string& addr, int port)
{
   _port = port;

   struct mg_connection* nc = mg_bind(&_mgr, std::to_string(port).c_str(), request_handler);

   mg_set_protocol_http_websocket(nc);

   _is_running = true;

   while (is_running()) 
   {
      mg_mgr_poll(&_mgr, 1000);
   }
   return std::error_code();
}

std::error_code MongooseServer::serve(struct mg_connection* connection, struct http_message* hm)
{
   std::unique_ptr<MongooseServerConnection> conn = std::make_unique<MongooseServerConnection>(connection, hm, _RequestHandlers);

   conn->accept();

   return std::error_code();
}

} // http
} // net
} // orion

