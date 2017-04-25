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
#include <orion/net/http/Server.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::net;
using namespace orion::net::http;


class HelloRequestHandler : public http::RequestHandler
{
public:
   HelloRequestHandler() : http::RequestHandler("/")
   {
   }

   virtual ~HelloRequestHandler() = default;

   static std::unique_ptr<http::RequestHandler> create()
   {
      return std::make_unique<HelloRequestHandler>();
   }
   
protected:
   virtual std::error_code on_get(const Request& request, Response& response)
   {
      response.header("Content-Type", "text/plain; charset=utf-8");
      response.header("Connection", "close");

      std::ostream o(response.rdbuf());

      o << "Hello there";

      return std::error_code();
   }
};

class WorldRequestHandler : public http::RequestHandler
{
public:
   WorldRequestHandler() : http::RequestHandler("/")
   {
   }

   virtual ~WorldRequestHandler() = default;

   static std::unique_ptr<http::RequestHandler> create()
   {
      return std::make_unique<WorldRequestHandler>();
   }
   
protected:
   virtual std::error_code on_get(const Request& request, Response& response)
   {
      response.header("Content-Type", "text/plain; charset=utf-8");
      response.header("Connection", "close");
      
      std::ostream o(response.rdbuf());

      o << "World turns round";

      return std::error_code();
   }
};

void setup_logger(std::fstream& file_stream)
{
   auto cout_handler = StreamOutputHandler::create(std::cout);
   auto file_handler = StreamOutputHandler::create(file_stream);

   file_handler->set_formatter(MultilineFormatter::create());

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

   auto server = http::Server::create();

   if (server == nullptr)
   {
      LOG(Info) << "Server error...";  
      return EXIT_FAILURE; 
   }

   server->add_handler(WorldRequestHandler::create());
   server->add_handler(HelloRequestHandler::create());

   std::cout << "Server listening on port: 9080\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9080);

      if (ec)
         LOG(Error) << ec;
   }
   catch (const std::exception& e)
   {
      LOG_EXCEPTION(e);
   }

   LOG_END();
   return EXIT_SUCCESS;
}
