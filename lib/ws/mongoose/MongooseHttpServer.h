/*
 * MongooseHttpServer.h
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

#ifndef MONGOOSEHTTPSERVER_H
#define MONGOOSEHTTPSERVER_H

#include <string>
#include <orion/ws/HttpServer.h>
#include <ws/mongoose/mongoose.h>

namespace orion
{
namespace ws
{
namespace mongoose
{

class MongooseHttpServer : public HttpServer
{
public:
   virtual ~MongooseHttpServer();

   virtual void start();
   virtual void stop();

   virtual void send_response(Response::SharedPtr response);
   virtual void send_response(Response::SharedPtr response, struct mg_connection* connection);

   static Server::SharedPtr create();

protected:
   MongooseHttpServer();

   std::string make_port_list();

private:
   bool _is_running;
   struct mg_context* _ctx;
};

} // mongoose
} // ws
} // orion
#endif 
