//
// hello-http-server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/http/Server.h>
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

   std::ostream o(response.body_rdbuf());

   o << "Hello there";

   return std::error_code();
}

std::error_code world(const http::Request& request, http::Response& response)
{
   response.header("Content-Type", "text/plain; charset=utf-8");
   response.header("Connection", "close");

   std::ostream o(response.body_rdbuf());

   o << "World turns round";

   return std::error_code();
}

void setup_logger(std::fstream& file_stream)
{
   auto cout_handler = make_stream_output_handler(std::cout);
   auto file_handler = make_stream_output_handler(file_stream);

   file_handler->set_formatter(make_multiline_formatter());

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

   auto server = std::make_unique<http::Server>();

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
      std::error_code ec = server->listen_and_serve({"0.0.0.0"_ipv4, 9080});

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
