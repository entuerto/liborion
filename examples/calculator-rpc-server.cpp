//
// calculator-rpc-server.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Log.h>
#include <orion/Utils.h>
#include <orion/net/rpc/Rpc.h>
#include <orion/net/tcp/Listener.h>

#include <cstdio>
#include <iostream>
#include <mutex>
#include <sstream>
#include <tuple>

using namespace orion;
using namespace orion::log;
using namespace orion::net;

//--------------------------------------------------------------------------------------------------
// Creating a rpc methods
//--------------------------------------------------------------------------------------------------

class Add : public rpc::Method<Add, int(int, int)>
{
public:
   static std::string name() { return "add"; }

   static std::string description() { return ""; }

   static Version version() { return Version{1, 0}; }
};

int add_int(int x, int y)
{
   log::debug("add_int(", x, ", ", y, ")");
   return x + y;
}

class Multiply : public rpc::Method<Multiply, int(int, int)>
{
public:
   static std::string name() { return "multiply"; }

   static std::string description() { return ""; }

   static Version version() { return Version{1, 0}; }
};

int multiply_int(int x, int y)
{
   log::debug("multiply_int(", x, ", ", y, ")");
   return x * y;
}

//--------------------------------------------------------------------------------------------------
// Tcp Service
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Setup the logger options
//--------------------------------------------------------------------------------------------------
void setup_logger()
{
   auto cout_handler = make_stream_output_handler(std::cout);

   Logger& logger = default_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
}

//--------------------------------------------------------------------------------------------------
// Main function
//--------------------------------------------------------------------------------------------------
int main()
{
   setup_logger();

   log::start();

   rpc::Service<rpc::Tcp<rpc::StringCodec>> calculator("Calculator", Version{1, 0});

   calculator.register_method<Add>(add_int);
   calculator.register_method<Multiply>(multiply_int);

   calculator.dump();

   

   //rpc::Dispatch<Tcp> disp;

   //disp.call<Add>(4, 5);

   log::write("Server listening on port: 9000");

   try
   {
      calculator.listen_and_serve(EndPoint{"0.0.0.0"_ipv4, 9000});
   }
   catch (const std::exception& e)
   {
      log::exception(e);
   }
   
   log::shutdown();
   return EXIT_SUCCESS;
}
