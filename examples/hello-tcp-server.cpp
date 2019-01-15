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

class RawHandler : public tcp::Handler
{
public:
   ~RawHandler() = default;
   
   std::error_code on_read(asio::streambuf& b) override
   {
      std::cout << "Received :\n";

      std::cout << &b << "\n";

      return {};
   }

   std::error_code on_write(asio::streambuf& b) override
   {
      std::ostream o(&b);

      o << "Welcome" << '\n';
      return {};
   }

};

using TcpListener = tcp::Listener<tcp::Connection, RawHandler>;

int main()
{
   log::setup_logger(log::Level::Debug);

   log::start();

   std::cout << "Server listening on port: 9000\n";

   try
   {
      asio::io_context io_context;

      std::make_shared<TcpListener>(io_context, EndPoint{"0.0.0.0"_ipv4, 9000}, RawHandler{})->start();

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
