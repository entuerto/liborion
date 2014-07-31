// main.cpp
//
// Copyright 2009 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include <cstdio>
#include <fstream>
#include <iostream>

#include <orion/Logging.h>
#include <orion/ws/Server.h>
#include <orion/ws/HttpServer.h>
#include <orion/ws/Response.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::ws;


class HelloRequestListener : public RequestListener
{
public:
   HelloRequestListener() : RequestListener("/")
   {
   }

   virtual ~HelloRequestListener()
   {
   }

   static RequestListener::SharedPtr create()
   {
      return RequestListener::SharedPtr(new HelloRequestListener);
   }
   
protected:
   virtual Response::SharedPtr on_get(Request::SharedPtr request)
   {
      return Response::create("Hello");
   }

   virtual Response::SharedPtr on_post(Request::SharedPtr request)
   {
      return Response::create_400();
   }

   virtual Response::SharedPtr on_put(Request::SharedPtr request) 
   {
      return Response::create_400();
   }

   virtual Response::SharedPtr on_delete(Request::SharedPtr request)
   {
      return Response::create_400();
   }
};

class WorldRequestListener : public RequestListener
{
public:
   WorldRequestListener() : RequestListener("/")
   {
   }

   virtual ~WorldRequestListener()
   {
   }

   static RequestListener::SharedPtr create()
   {
      return RequestListener::SharedPtr(new WorldRequestListener);
   }
   
protected:
   virtual Response::SharedPtr on_get(Request::SharedPtr request)
   {
      return Response::create("World");
   }

   virtual Response::SharedPtr on_post(Request::SharedPtr request)
   {
      return Response::create_400();
   }

   virtual Response::SharedPtr on_put(Request::SharedPtr request) 
   {
      return Response::create_400();
   }

   virtual Response::SharedPtr on_delete(Request::SharedPtr request)
   {
      return Response::create_400();
   }
};

void setup_logger(std::fstream& file_stream)
{
   StreamOutputHandler::SharedPtr cout_handler = StreamOutputHandler::create(std::cout);
   StreamOutputHandler::SharedPtr file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Logger::Debug);
   logger.output_handlers().push_back(cout_handler);
   logger.output_handlers().push_back(file_handler);
}

int main (int argc, char** argv)
{
   std::fstream fout("hello-server.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   Server::SharedPtr server = HttpServer::create(9080);

   server->add_request_listener(WorldRequestListener::create());
   server->add_request_listener(HelloRequestListener::create());

   LOG(Info) << "Server starting...";

   server->start();

   LOG_END();
   return EXIT_SUCCESS;
}
