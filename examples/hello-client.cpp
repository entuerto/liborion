// hello-client.cpp
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <cstdio>
#include <fstream>
#include <iostream>

#include <orion/Logging.h>
#include <orion/net/http/Client.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::net;
using namespace orion::net::http;

void setup_logger()
{
   auto cout_handler = StreamOutputHandler::create(std::cout);

   Logger& logger = Logger::get_logger();

   logger.level(Level::Debug);
   logger.output_handlers().push_back(std::move(cout_handler));
}

int main ()
{
   setup_logger();

   LOG_START();

   auto r = http::Post(Url{"http://127.0.0.1:9080/"}); 

   try
   {
      LOG_WRITE() << "Server response:";
   
      auto response =  r.get();
   
      LOG_WRITE() << response;
      LOG_WRITE() << response.rdbuf();
   }
   catch (const std::future_error& e)
   {
      LOG(Error) << e.code();
   }
   catch (const std::system_error& e)
   {
      LOG(Error) << e.code();
   }

   LOG_END();
   return EXIT_SUCCESS;
}