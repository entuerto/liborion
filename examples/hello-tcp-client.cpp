//
// hello-client.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/AsyncService.h>
#include <orion/Log.h>
#include <orion/System.h>
#include <orion/net/tcp/Session.h>
#include <orion/net/tcp/Utils.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

using namespace orion;
using namespace orion::net;

using namespace std::chrono_literals;

int main()
{
   log::setup_logger(log::Level::Debug);

   log::start();

   AsyncService as(1);
/*
   auto& signals = as.signals();

   signals.add(SIGINT);
   signals.add(SIGILL);
   signals.add(SIGFPE);
   signals.add(SIGSEGV);
   signals.add(SIGTERM);

   signals.async_wait([&](std::error_code ec, int signo) {
      if (not ec)
      {
         //as.stop();
         return;
      }

      log::error(ec);
      sys::write_stack_trace(std::cout);
   });
*/
   tcp::Session s(as.io_context());

   s.on_connect([&](const std::error_code& ec)
   {
      if (ec)
      {
         log::error("Connection error: ", ec);
         as.stop();
         return;
      }

      log::info("Connected!");

      asio::streambuf buf;

      std::iostream stream(&buf);

      stream << "RPC-MSG 1.0 Char Request 1 1 add 4 5" << "\n";

      s.write(&buf);
   });

   s.on_read([&](const std::error_code& ec, asio::streambuf& buf)
   {
      if (ec)
      {
         log::error("Read error: ", ec);
         s.close();
         return;
      }

      std::cout << "Read: " << &buf << "\n";

      s.close();
      as.stop();
   });

   s.on_write([&](const std::error_code& ec, std::size_t bytes_sent)
   {
      if (ec)
      {
         log::error("Write error: ", ec);
         s.close();
         return;
      }

      std::cout << "Bytes sent: " << bytes_sent << "\n";
   });

   try
   {
      s.connect(EndPoint{"127.0.0.1"_ipv4, 9000});
       
      as.run();    
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
