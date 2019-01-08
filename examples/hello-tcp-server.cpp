//
// hello-tcp-server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/tcp/Connection.h>
#include <orion/net/tcp/Listener.h>
#include <orion/net/tcp/Utils.h>

#include <cstdio>
#include <iostream>

using namespace orion;
using namespace orion::net;

using TcpListener = tcp::Listener<tcp::Connection, tcp::Handler>;

int main()
{
   log::setup_logger(log::Level::Debug);

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

      std::make_shared<TcpListener>(io_context, EndPoint{"0.0.0.0"_ipv4, 9000}, handler)->start();

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
