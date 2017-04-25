// main.cpp
//
// Copyright 2009 tomas <tomasp@videotron.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include <cstdio>
#include <fstream>
#include <iostream>

#include <orion/Log.h>
#include <orion/net/Server.h>
#include <orion/net/http/Server.h>
#include <orion/net/rpc/JsonRequestHandler.h>
#include <orion/net/rpc/JsonMethod.h>

#include <jsoncpp/json/json.h>

using namespace orion;
using namespace orion::log;
using namespace orion::net;
using namespace orion::net::http;
using namespace orion::net::rpc;

//-----------------------------------------------------------------------------------
// Creating a json-rpc method
//-----------------------------------------------------------------------------------
class AddMethod : public JsonMethod
{
public:
   AddMethod() : 
      JsonMethod("add", "Add to numbers together.")
   {
   }

   virtual ~AddMethod() = default;

   virtual Error operator()(Json::Value& json_request, Json::Value& json_result) override
   {
      int param1 = 0;
      int param2 = 0;

      if (not json_request.isMember("params"))
         return make_error(JsonErrc::InvalidParams); //("Must specify two parameters.");

      Json::Value params = json_request["params"];

      if (params.isArray())
      {
         param1 = params[0u].asInt();
         param2 = params[1u].asInt();
      }
      else
      {
         param1 = params["a1"].asInt();
         param2 = params["a2"].asInt();
      }

      LOG(Debug) << "Add " 
                 << param1 
                 << ", "
                 << param2;

      json_result = param1 + param2;
      return Error();
   }
};

//-----------------------------------------------------------------------------------
// Free function example
//-----------------------------------------------------------------------------------
Error substract(Json::Value& json_request, Json::Value& json_result)
{
    int param1 = 0;
    int param2 = 0;

    if (not json_request.isMember("params"))
       return make_error(JsonErrc::InvalidParams); //("Must specify two parameters.");

    Json::Value params = json_request["params"];

    if (params.isArray())
    {
       param1 = params[0u].asInt();
       param2 = params[1u].asInt();
    }
    else
    {
       param1 = params["a1"].asInt();
       param2 = params["a2"].asInt();
    }

    json_result = param1 - param2;
    return Error();
}

//-----------------------------------------------------------------------------------
// Object function example
//-----------------------------------------------------------------------------------
class Echo
{
public:

   static Error answer(Json::Value& /* json_request */, Json::Value& json_result)
   {
      json_result = "Hello World!";
      return Error();
   }
};

//-----------------------------------------------------------------------------------
// Shutdown example
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// Setup the logger options
//-----------------------------------------------------------------------------------
void setup_logger(std::fstream& file_stream)
{
   auto cout_handler = std::make_unique<StreamOutputHandler>(std::cout);
   auto file_handler = std::make_unique<StreamOutputHandler>(file_stream);

   file_handler->set_formatter(std::make_unique<MultilineFormatter>());

   Logger& logger = Logger::get_logger();

   logger.level(Level::Debug);
   logger.add_output_handler(std::move(cout_handler));
   logger.add_output_handler(std::move(file_handler));
}

//-----------------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------------
int main ()
{
   std::fstream fout("add-json-rpc-server.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   log::start();

   auto server = http::Server::create();

   auto rl = std::make_unique<JsonRequestHandler>("/");

   rl->register_method(AddMethod()); 
   rl->register_method(JsonMethod{"substract", "", substract}); 
   rl->register_method(JsonMethod{"answer", "", Echo::answer}); 


   server->add_handler(std::move(rl));

   std::cout << "Server listening on port: 9080\n";

   try
   {
      std::error_code ec = server->listen_and_serve("", 9080);

      if (ec)
         LOG(Error) << ec;
   }
   catch (const std::exception& e)
   {
      LOG_EXCEPTION(e);
   }

   log::shutdown();
   return EXIT_SUCCESS;
}
