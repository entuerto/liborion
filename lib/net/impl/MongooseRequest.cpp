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

#include <net/impl/MongooseRequest.h>

#include <cstdlib>
#include <orion/Logging.h>
#include <orion/StringUtils.h>
#include <orion/net/IPAddress.h>

namespace orion
{
namespace net
{
namespace mongoose
{
using namespace orion::logging;

static std::string as_string(struct mg_str& str)
{
   return std::string(str.p, str.len);
}

MongooseRequest::MongooseRequest(struct mg_connection* connection, struct http_message* hm) :
   Request(),
   _connection(connection),
   _hm(hm)
{
   char buffer[100];
   mg_conn_addr_to_str(connection, buffer, 100, MG_SOCK_STRINGIFY_REMOTE |
                                                MG_SOCK_STRINGIFY_IP     |
                                                MG_SOCK_STRINGIFY_PORT);
   LOG(Debug) << "Remote address:" << buffer;
   
   StringVector remote = split(buffer,  ':');

   _remote_address = TcpAddress::create(remote[0], std::atoi(remote[1].c_str()));

   mg_conn_addr_to_str(connection, buffer, 100, MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
   
   LOG(Debug) << "Host address:" << buffer;

   StringVector host = split(buffer,  ':');

   _host_address = TcpAddress::create(host[0], std::atoi(host[1].c_str()));
   
   read_data(hm);
}

MongooseRequest::~MongooseRequest()
{
}

//! "GET", "POST", etc
std::string MongooseRequest::method() const
{
   return as_string(_hm->method);
}

//! URL-decoded URI
std::string MongooseRequest::uri() const
{
   return as_string(_hm->uri);
}

//! E.g. "1.0", "1.1"
std::string MongooseRequest::http_version() const
{
   return as_string(_hm->proto);
}
   
//! URL part after '?', not including '?', or NULL
std::string MongooseRequest::query_string() const
{
   return as_string(_hm->query_string);
}

//! Authenticated user, or NULL if no auth used
std::string MongooseRequest::remote_user() const
{
   return "";
}
   
bool MongooseRequest::is_authenticated()  const
{
   return false; // @TODO
}

bool MongooseRequest::is_secure_connection() const
{
   return false; // @TODO
}

std::string MongooseRequest::header(const std::string& name) const
{
   auto value = mg_get_http_header(_hm, name.c_str());
   return as_string(*value);
}

void MongooseRequest::read_data(struct http_message* hm)
{
   _data = as_string(hm->body);
}

std::unique_ptr<Request> MongooseRequest::create(struct mg_connection* connection, struct http_message* hm)
{
   return std::make_unique<MongooseRequest>(connection, hm); 
}

} // mongoose
} // net
} // orion

