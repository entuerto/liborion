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

#include <orion/ws/Server.h>
#include <orion/ws/Response.h>
#include <orion/ws/Request.h>
#include <orion/ws/RequestListener.h>

namespace orion
{
namespace ws
{
typedef std::map<int, RequestListener::SharedPtr> listenerMap;

//! This class provides an embedded HTTP Server
/*!
   This class provides an embedded HTTP Server, based on Mongoose, to handle incoming Requests 
   and send HTTP 1.1 valid responses.
 */
class HttpServer : public Server
{
public:
   virtual ~HttpServer();

   virtual void add_request_listener(int port, RequestListener::SharedPtr listener);

   virtual Response::SharedPtr process_request(Request::SharedPtr request);
   
   static Server::SharedPtr create();

protected:
   HttpServer();

   listenerMap _RequestListeners;
};

} // ws
} // orion
#endif 
