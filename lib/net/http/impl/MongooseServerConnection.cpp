/*
 * MongooseRequest.cpp
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

#include <net/http/impl/MongooseServerConnection.h>

#include <orion/Log.h>
#include <orion/StringUtils.h>
#include <orion/net/IPAddress.h>
#include <cstdlib>

using namespace orion::log;

namespace orion
{
namespace net
{
namespace http
{
/*
static std::string as_string(struct mg_str& str)
{
   return std::string(str.p, str.len);
}
*/
   
MongooseServerConnection::MongooseServerConnection(struct mg_connection* connection,
                                                   struct http_message* hm,
                                                   const Handlers& RequestHandlers)
   : tcp::Connection()
   , _connection(connection)
   , _hm(hm)
   , _RequestHandlers(RequestHandlers)
   , _request()
   , _response(StatusCode::OK)
{
   char buffer[100];
   mg_conn_addr_to_str(connection,
                       buffer,
                       100,
                       MG_SOCK_STRINGIFY_REMOTE | MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
   StringVector remote = split(buffer, ':');

   _remote_addr.reset(new IPAddress(IPv4::parse(remote[0]), std::atoi(remote[1].c_str())));

   mg_conn_addr_to_str(connection, buffer, 100, MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

   StringVector host = split(buffer, ':');

   _local_addr.reset(new IPAddress(IPv4::parse(host[0]), std::atoi(host[1].c_str())));
}

MongooseServerConnection::~MongooseServerConnection()
{
   close();
}

void MongooseServerConnection::close()
{
   // mg_close_conn(_connection);
}

void MongooseServerConnection::accept()
{
   LOG(Info) << boost::format("(%p) Connection accepted") % this;
   LOG(Info) << "   Remote address: " << _remote_addr->to_string();
   LOG(Info) << "   Local address:  " << _local_addr->to_string();

   do_read();
}

void MongooseServerConnection::do_read()
{
   _request = MongooseRequest(_hm);

   do_handler();
   do_write();
}

void MongooseServerConnection::do_handler()
{
   LOG_FUNCTION(Debug2, "MongooseServerConnection::do_handler()")

   LOG(Debug2) << _request;

   auto it = _RequestHandlers.find(_request.url().pathname());

   if (it != _RequestHandlers.end())
   {
      auto& h = it->second;

      auto ec = h(_request, _response);
      if (ec)
      {
         LOG(Error) << ec;

         _response = MongooseResponse(static_cast<StatusCode>(ec.value()));
      }
      return;
   }
   _response = MongooseResponse(StatusCode::BadRequest);
}

void MongooseServerConnection::do_write()
{
   auto b = _response.buffer();

   LOG(Debug2) << b;

   mg_send(_connection, b.c_str(), b.size());
}

} // http
} // net
} // orion
