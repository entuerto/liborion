/*
 * Server.h
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

#ifndef ORION_WS_SERVER_H
#define ORION_WS_SERVER_H

#include <string>
#include <orion/MemoryUtils.h>
#include <orion/ws/RequestListener.h>
#include <orion/ws/Response.h>

namespace orion
{
namespace ws 
{
//! Server interface declaration
class Server
{
public:
   DECLARE_POINTERS(Server)

   virtual ~Server() {}

   virtual void start() =0;
   virtual void stop() =0;

   virtual bool is_running() const =0;

   virtual void add_request_listener(int port, RequestListener::SharedPtr listener) =0;

   virtual void send_response(Response::SharedPtr response) =0;
};

} // ws
} // orion
#endif 

