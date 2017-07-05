// hello-client.cpp
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/Log.h>
#include <orion/net/http/Client.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>

#include <cstdio>
#include <fstream>
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

   auto r = http::Post(Url{"http://127.0.0.1:9080/hello"});

   try
   {
      log::write("Server response:");

      auto response = r.get();

      log::write(response);
      log::write(response.rdbuf());
   }
   catch (const std::future_error& e)
   {
      LOG(Error) << e.code();
   }
   catch (const std::system_error& e)
   {
      LOG(Error) << e.code();
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
