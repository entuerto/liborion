/*
 * MongooseServer.h
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

#ifndef ORION_NET_MONGOOSESERVER_H
#define ORION_NET_MONGOOSESERVER_H

#include <string>

#include <orion/net/http/Server.h>

#include <mongoose/mongoose.h>

namespace orion
{
namespace net
{
namespace http
{

class MongooseServer : public Server
{
public:
   NO_COPY(MongooseServer)

   MongooseServer();
   virtual ~MongooseServer();

   void shutdown() override;

   std::error_code listen_and_serve(const std::string& addr, int port) override;

   std::error_code serve(struct mg_connection* connection, struct http_message* hm);

private:
   struct mg_mgr _mgr; 
};

} // http
} // net
} // orion
#endif 
