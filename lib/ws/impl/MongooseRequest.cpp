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

#include <ws/impl/MongooseRequest.h>

#include <cstdlib>
#include <orion/StringUtils.h>

namespace orion
{
namespace ws
{
namespace mongoose
{

MongooseRequest::MongooseRequest(struct mg_connection* connection) :
   Request(),
   _method(),
   _uri(),
   _http_version(),
   _query_string(),
   _remote_user()
{
   _method = connection->request_method;
   _uri = connection->uri;
   _http_version = connection->http_version;
   _query_string = connection->query_string == nullptr ? "" : connection->query_string;
   //_remote_user = ;

   _http_header = make_header_map(connection);

   _remote_address = InetAddress::create(connection->remote_ip, connection->remote_port);

   StringVector host = split(this->header("Host"),  ':');

   if (host.size() == 2)
      _host_address = InetAddress::create(host[0], std::atoi(host[1].c_str()));

   read_data(connection);
}

MongooseRequest::~MongooseRequest()
{
}

//! "GET", "POST", etc
std::string MongooseRequest::method() const
{
   return _method;
}

//! URL-decoded URI
std::string MongooseRequest::uri() const
{
   return _uri;
}

//! E.g. "1.0", "1.1"
std::string MongooseRequest::http_version() const
{
   return _http_version;
}
   
//! URL part after '?', not including '?', or NULL
std::string MongooseRequest::query_string() const
{
   return _query_string;
}

//! Authenticated user, or NULL if no auth used
std::string MongooseRequest::remote_user() const
{
   return _remote_user;
}
   
bool MongooseRequest::is_authenticated()  const
{
   return false; // TODO
}

bool MongooseRequest::is_secure_connection() const
{
   return false; // TODO
}

HeaderMap MongooseRequest::make_header_map(const struct mg_connection* connection)
{
   HeaderMap hm;

   for (int i = 0; i < connection->num_headers; i++)
   {
      hm.insert(std::pair<std::string, std::string>(connection->http_headers[i].name,
                                                    connection->http_headers[i].value));
   }

   return hm;
}

void MongooseRequest::read_data(struct mg_connection* connection)
{
   if (connection->content_len == 0)
      return;

   _data = connection->content;
}

Request::SharedPtr MongooseRequest::create(struct mg_connection* connection)
{
   return std::make_shared<MongooseRequest>(connection); 
}

} // mongoose
} // ws
} // orion

