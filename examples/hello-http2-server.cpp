//
// hello-http2-server.h
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/http2/Server.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/RequestMux.h>
#include <orion/net/http/Response.h>

#include <clara/clara.hpp>
#include <fmt/format.h>

#include <cstdio>
#include <fstream>
#include <iostream>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

std::error_code hello(const http::Request& request, http::Response& response)
{
   response.header(http::Field::ContentType, "text/plain; charset=utf-8");

   std::ostream o(response.body());

   o << "Hello there";

   return {};
}

std::error_code world(const http::Request& request, http::Response& response)
{
   response.header(http::Field::ContentType, "text/plain; charset=utf-8");

   std::ostream o(response.body());

   o << "World turns round";

   return std::error_code();
}

bool parse_cmd_options(int argc, char* argv[], uint16_t& port)
{
   using namespace clara;

   bool show_help = false;

   auto options = Help(show_help)
                | Opt(port, "port")["-p"]("port to listen"); 

   auto result = options.parse(Args(argc, argv));
   if (not result)
   {
      std::cerr << "Error: \n" << result.errorMessage() << "\n";
      return false;
   }
   if (show_help)
   {
      options.writeToStream(std::cout);
      return false;
   }
   return true;
}

int main(int argc, char* argv[])
{
   uint16_t port = 9080;

   if (not parse_cmd_options(argc, argv, port))
      return EXIT_FAILURE;

   log::setup_logger(log::Level::Debug);

   log::start();

   http::RequestMux mux;

   mux.handle(http::Method{"GET"}, "/world", world);
   mux.handle(http::Method{"GET"}, "/hello", hello);

   http2::Server server = http2::make_server(std::move(mux));

   try
   {
      log::write(fmt::format("Server listening on port: {}\n", port));

      auto ec = server.listen_and_serve({"0.0.0.0"_ipv4, port});

      log::error_if(ec, ec);
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
