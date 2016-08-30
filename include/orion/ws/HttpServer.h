/*
 * HttpServer.h
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

#ifndef ORION_WS_HTTPSERVER_H
#define ORION_WS_HTTPSERVER_H

#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/ws/Server.h>
#include <orion/ws/Response.h>
#include <orion/ws/Request.h>
#include <orion/ws/RequestListener.h>

namespace orion
{
namespace ws
{
typedef std::map<std::string, std::unique_ptr<RequestListener>> listenerMap;

//! This class provides an embedded HTTP Server
/*!
   This class provides an embedded HTTP Server, based on Mongoose, to handle incoming Requests 
   and send HTTP 1.1 valid responses.
 */
class API_EXPORT HttpServer : public Server
{
public:
   NO_COPY(HttpServer)
   NO_MOVE(HttpServer)

   virtual ~HttpServer();

   virtual int port() const;

   virtual void add_request_listener(std::unique_ptr<RequestListener>&& listener);

   virtual std::unique_ptr<Response> process_request(const Request* request);
   
   static std::unique_ptr<Server> create(int port);

protected:
   HttpServer(int port);

   int _port;
   listenerMap _RequestListeners;
};

} // ws
} // orion
#endif 
