//
// hello-tcp-server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/tcp/Server.h>

#include <cstdio>
#include <iostream>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

void setup_logger()
{
   auto cout_handler = std::make_unique<StreamOutputHandler>(std::cout);

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

int main()
{
   setup_logger();

   log::start();

   auto server = std::make_unique<tcp::Server>();

   server->register_handler([&](std::streambuf* in, std::streambuf* out) {
      std::cout << "Received :\n";

      std::cout << in << "\n";

      std::ostream o(out);

      o << "Welcome" << "\n";

      return std::error_code();
   });

   std::cout << "Server listening on port: 9090\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9090);

      if (ec)
         log::error(ec);
   }
   catch (const std::system_error& e)
   {
      log::exception(e);
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
