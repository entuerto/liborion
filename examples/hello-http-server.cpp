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
#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Response.h>

#include <cstdio>
#include <fstream>
#include <iostream>

using namespace orion;
using namespace orion::log;
using namespace orion::net;
using namespace orion::net::http;

std::error_code hello(const http::Request& request, http::Response& response)
{
   response.header(Field::ContentType, "text/plain; charset=utf-8");
   response.header(Field::Connection, "close");

   std::ostream o(response.body_rdbuf());

   o << "Hello there";

   return std::error_code();
}

std::error_code world(const http::Request& request, http::Response& response)
{
   response.header(Field::ContentType, "text/plain; charset=utf-8");
   response.header(Field::Connection, "close");

   std::ostream o(response.body_rdbuf());

   o << "World turns round";

   return std::error_code();
}

void setup_logger()
{
   auto cout_handler = make_stream_output_handler(std::cout);

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

int main()
{
   setup_logger();

   log::start();

   http::Server server;

   RequestMux mux;

   mux.handle(Method{"GET"}, "/world", world);
   mux.handle(Method{"GET"}, "/hello", hello);

   log::write("Server listening on port: 9080\n");

   try
   {
      auto ec = server.listen_and_serve({"0.0.0.0"_ipv4, 9080}, std::move(mux));

      log::error_if(ec, ec);
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
