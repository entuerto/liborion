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

#include <orion/Logging.h>
#include <orion/ws/Server.h>
#include <orion/ws/HttpServer.h>
#include <orion/ws/JsonRpcRequestListener.h>
#include <orion/ws/JsonRpcMethod.h>
#include <orion/ws/JsonRpcMethodWrapper.h>

#include <jsoncpp/json/json.h>

using namespace orion;
using namespace orion::logging;
using namespace orion::ws;

//-----------------------------------------------------------------------------------
// Creating a json-rpc method
//-----------------------------------------------------------------------------------
class AddMethod : public JsonRpcMethod
{
public:
   AddMethod() : JsonRpcMethod()
   {
   }

   virtual ~AddMethod() = default;

   virtual std::string name() const
   {
      return "add";
   }

   virtual std::string description() const
   {
      return "Add to numbers together.";
   }

   virtual std::unique_ptr<JsonRpcError> call(Json::Value& json_request, Json::Value& json_result)
   {
      int param1 = 0;
      int param2 = 0;

      if (not json_request.isMember("params"))
         return JsonRpcError::create_invalid_params("Must specify two parameters.");

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

      json_result = param1 + param2;
      return nullptr;
   }

   static std::unique_ptr<RpcMethod> create()
   {
      return std::make_unique<AddMethod>();
   }
};

//-----------------------------------------------------------------------------------
// Free function example
//-----------------------------------------------------------------------------------
std::unique_ptr<JsonRpcError> substract(Json::Value& json_request, Json::Value& json_result)
{
    int param1 = 0;
    int param2 = 0;

    if (not json_request.isMember("params"))
       return JsonRpcError::create_invalid_params("Must specify two parameters.");

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
    return nullptr;
}

//-----------------------------------------------------------------------------------
// Object function example
//-----------------------------------------------------------------------------------
class Echo
{
public:

   static std::unique_ptr<JsonRpcError> answer(Json::Value& /* json_request */, Json::Value& json_result)
   {
      json_result = "Hello World!";
      return nullptr;
   }
};

//-----------------------------------------------------------------------------------
// Shutdown example
//-----------------------------------------------------------------------------------
struct ShutdownServer
{
   static std::unique_ptr<JsonRpcError> quit(Json::Value& /* json_request */, Json::Value& json_result)
   {
      ShutdownServer::server->stop();

      json_result = "Stopping server";
      
      return nullptr;
   }

   static std::unique_ptr<Server> server;
};

std::unique_ptr<Server> ShutdownServer::server;

//-----------------------------------------------------------------------------------
// Setup the logger options
//-----------------------------------------------------------------------------------
void setup_logger(std::fstream& file_stream)
{
   auto cout_handler = StreamOutputHandler::create(std::cout);
   auto file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Level::Debug);
   logger.output_handlers().push_back(std::move(cout_handler));
   logger.output_handlers().push_back(std::move(file_handler));
}

//-----------------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------------
int main ()
{
   std::fstream fout("add-json-rpc-server.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   ShutdownServer::server = HttpServer::create(9090);

   auto rl = JsonRpcRequestListener::create("/");

   rl->register_method("add", AddMethod::create()); 
   rl->register_method("substract", JsonRpcMethodWrapper::create(substract, "substract", "Substract two numbers")); 
   rl->register_method("answer", JsonRpcMethodWrapper::create(Echo::answer, "answer")); 

   rl->register_method("shutdown", JsonRpcMethodWrapper::create(ShutdownServer::quit, "shutdown")); 

   ShutdownServer::server->add_request_listener(std::move(rl));

   LOG(Info) << "Server starting...";

   ShutdownServer::server->start();

   LOG_END();
   return EXIT_SUCCESS;
}
