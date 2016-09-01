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
#include <orion/net/Server.h>
#include <orion/net/HttpServer.h>
#include <orion/net/Response.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::net;


class HelloRequestListener : public RequestListener
{
public:
   HelloRequestListener() : RequestListener("/")
   {
   }

   virtual ~HelloRequestListener() = default;

   static std::unique_ptr<RequestListener> create()
   {
      return std::make_unique<HelloRequestListener>();
   }
   
protected:
   virtual std::unique_ptr<Response> on_get(const Request* /* request */)
   {
      return Response::create("Hello");
   }

   virtual std::unique_ptr<Response> on_post(const Request* /* request */)
   {
      return Response::create_400();
   }

   virtual std::unique_ptr<Response> on_put(const Request* /* request */) 
   {
      return Response::create_400();
   }

   virtual std::unique_ptr<Response> on_delete(const Request* /* request */)
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

   virtual ~WorldRequestListener() = default;

   static std::unique_ptr<RequestListener> create()
   {
      return std::make_unique<WorldRequestListener>();
   }
   
protected:
   virtual std::unique_ptr<Response> on_get(const Request* /* request */)
   {
      return Response::create("World");
   }

   virtual std::unique_ptr<Response> on_post(const Request* /* request */)
   {
      return Response::create_400();
   }

   virtual std::unique_ptr<Response> on_put(const Request* /* request */) 
   {
      return Response::create_400();
   }

   virtual std::unique_ptr<Response> on_delete(const Request* /* request */)
   {
      return Response::create_400();
   }
};

void setup_logger(std::fstream& file_stream)
{
   auto cout_handler = StreamOutputHandler::create(std::cout);
   auto file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Level::Debug);
   logger.output_handlers().push_back(std::move(cout_handler));
   logger.output_handlers().push_back(std::move(file_handler));
}

int main ()
{
   std::fstream fout("hello-server.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   auto server = HttpServer::create(9080);

   server->add_request_listener(WorldRequestListener::create());
   server->add_request_listener(HelloRequestListener::create());

   LOG(Info) << "Server starting...";

   server->start();

   LOG_END();
   return EXIT_SUCCESS;
}
