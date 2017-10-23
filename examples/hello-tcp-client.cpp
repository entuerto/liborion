// hello-client.cpp
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/AsyncService.h>
#include <orion/Log.h>
#include <orion/net/tcp/Session.h>
#include <orion/net/tcp/Utils.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

using namespace std::chrono_literals;

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

   std::unique_ptr<std::streambuf> buf = tcp::make_buffer();

   std::iostream stream(buf.get());

   stream << "hello" << "\n";

   tcp::Session s;

   s.on_connect([&](const std::error_code& ec)
   {
      if (ec)
      {
         log::error("Connection error: ", ec);
         return;
      }

      log::info("Connected!");

      s.write(buf.get());
   });

   s.on_read([&](const std::error_code& ec, std::streambuf* buf)
   {
      if (ec)
      {
         log::error("Read error: ", ec);
         s.close();
         return;
      }

      std::cout << "Read: " << buf << "\n";

      s.close();
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
      s.connect("127.0.0.1", 9090);
      
 
      AsyncService::run();    
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
