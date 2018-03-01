//
// hello-tcp-server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/tcp/Listener.h>

#include <cstdio>
#include <iostream>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

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

   auto handler = [&](asio::streambuf& in, asio::streambuf& out) {
      std::cout << "Received :\n";

      std::cout << &in << "\n";

      std::ostream o(&out);

      o << "Welcome"
        << "\n";

      return std::error_code();
   };

   std::cout << "Server listening on port: 9090\n";

   try
   {
      asio::io_context io_context;

      std::make_shared<tcp::Listener>(io_context, EndPoint{"0.0.0.0"_ipv4, 9000}, handler)->start();

      io_context.run();
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
