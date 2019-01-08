//
// hello-http-client.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/net/http/Client.h>
#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>

#include <cstdio>
#include <fstream>
#include <iostream>

using namespace orion;
using namespace orion::net;

void sync_call()
{
   auto r = http::Get(Url{"http://127.0.0.1:9080/world"});

   try
   {
      log::write("Server response:");

      auto response = r.get();

      log::write(response);
      log::write(response.body());
   }
   catch (const std::future_error& e)
   {
      LOG(Error) << e.code();
   }
   catch (const std::system_error& e)
   {
      LOG(Error) << e.code();
   }
}

void async_call()
{
   log::write("Asynchronous call\n");

   auto res_handler = [](const http::Response& response) {
      //auto& r = const_cast<http::Response&>(response);

      log::write("Server response:");
      log::write(response);
      log::write(response.body());
   };

   http::AsyncGet(Url{"http://127.0.0.1:9080/world"}, res_handler);
}

int main()
{
   log::setup_logger(log::Level::Debug);

   log::start();

   sync_call();
   async_call();

   log::shutdown();
   return EXIT_SUCCESS;
}
