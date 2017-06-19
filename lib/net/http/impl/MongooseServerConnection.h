/*
 * Request.h
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

#ifndef ORION_NET_MONGOOSESERVERCONNECTION_H
#define ORION_NET_MONGOOSESERVERCONNECTION_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/RequestHandler.h>
#include <orion/net/http/Utils.h>
#include <orion/net/tcp/Connection.h>

#include <net/http/impl/MongooseRequest.h>
#include <net/http/impl/MongooseResponse.h>

#include <mongoose/mongoose.h>

#include <memory>
#include <string>

namespace orion
{
namespace net
{
namespace http
{

class MongooseServerConnection : public tcp::Connection
{
public:
   NO_COPY(MongooseServerConnection)

   MongooseServerConnection(struct mg_connection* connection,
                            struct http_message* hm,
                            const Handlers& RequestHandlers);

   virtual ~MongooseServerConnection();

   virtual void close() override;

   void accept();

private:
   void do_read();
   void do_handler();
   void do_write();

   struct mg_connection* _connection;
   struct http_message* _hm;

   /// Request handlers
   Handlers _RequestHandlers;

   MongooseRequest _request;
   MongooseResponse _response;
};

} // http
} // net
} // orion

#endif // ORION_NET_MONGOOSESERVERCONNECTION_H
