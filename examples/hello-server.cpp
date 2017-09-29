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

#include <orion/Log.h>
#include <orion/net/Server.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Server.h>

#include <cstdio>
#include <fstream>
#include <iostream>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

std::error_code hello(const http::Request& request, http::Response& response)
{
   response.header("Content-Type", "text/plain; charset=utf-8");
   response.header("Connection", "close");

   std::ostream o(response.rdbuf());

   o << "Hello there";

   return std::error_code();
}

std::error_code world(const http::Request& request, http::Response& response)
{
   response.header("Content-Type", "text/plain; charset=utf-8");
   response.header("Connection", "close");

   std::ostream o(response.rdbuf());

   o << "World turns round";

   return std::error_code();
}

void setup_logger(std::fstream& file_stream)
{
   auto cout_handler = std::make_unique<StreamOutputHandler>(std::cout);
   auto file_handler = std::make_unique<StreamOutputHandler>(file_stream);

   file_handler->set_formatter(std::make_unique<MultilineFormatter>());

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
   logger.add_output_handler(std::move(file_handler));
}

int main()
{
   std::fstream fout("hello-server.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   log::start();

   auto server = http::Server::create();

   if (server == nullptr)
   {
      LOG(Info) << "Server error...";
      return EXIT_FAILURE;
   }

   server->add_handler("/world", world);
   server->add_handler("/hello", hello);

   std::cout << "Server listening on port: 9080\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9080);

      if (ec)
         LOG(Error) << ec;
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
