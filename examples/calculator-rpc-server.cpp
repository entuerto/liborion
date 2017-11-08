//
// calculator-rpc-server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Utils.h>
#include <orion/net/Server.h>
#include <orion/net/tcp/Server.h>
#include <orion/net/rpc/Rpc.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <tuple>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

//-----------------------------------------------------------------------------------
// Creating a rpc methods
//-----------------------------------------------------------------------------------
/*
class CalculatorService : public rpc::Service
{
public:
   CalculatorService()
      : rpc::Service("Calculator", "", 1.0, "/")
   {
      register_method(rpc::Method{"add", "", 1.0});
      register_method(rpc::Method{"substract", "", 1.0});
   }

   virtual ~CalculatorService() = default;

   void add(const AddParams& params, AddResult& result) { result.value = params.a + params.b; }

   void substract(const SubstractParams& params, SubstractResult& result)
   {
      result.value = params.a - params.b;
   }
};
*/

class Add : public rpc::Method<int(int, int)> 
{
public:
   static std::string name()        { return "add"; }

   static std::string description() { return ""; }

   static Version version()         { return Version{1, 0}; }
};

int add_int(int x, int y)
{
   return x + y;
}

//-----------------------------------------------------------------------------------
// Setup the logger options
//-----------------------------------------------------------------------------------
void setup_logger()
{
   auto cout_handler = std::make_unique<StreamOutputHandler>(std::cout);

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

//-----------------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------------
int main()
{
   setup_logger();

   log::start();
/*
   auto server = http::Server::create();

   server->add_handler(make_http_handler(std::make_unique<CalculatorService>()));

   std::cout << "Server listening on port: 9080\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9080);

      if (ec)
         LOG(Error) << ec;
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   rpc::Service<std::iostream> s("Calculator", Version{1, 0});

   s.register_method<Add>(add_int);

   log::debug("Value: ");
*/
   auto server = std::make_unique<tcp::Server>();

   server->register_handler([&](std::streambuf* in, std::streambuf* out) {
      std::cout << "Received :\n";

      std::cout << in << "\n";

      std::ostream o(out);

      o << "Hello!";

      return std::error_code();
   });

   std::cout << "Server listening on port: 9000\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9000);

      if (ec)
         log::error(ec);
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
