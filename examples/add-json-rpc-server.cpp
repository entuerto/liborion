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
#include <condition_variable>
#include <mutex>
#include <fstream>
#include <iostream>

#include <orion/Logging.h>
#include <orion/ws/Server.h>
#include <orion/ws/HttpServer.h>
#include <orion/ws/JsonRpcRequestListener.h>
#include <orion/ws/JsonRpcMethod.h>
#include <orion/ws/JsonRpcMethodWrapper.h>

#include <ws/json/value.h>

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

   virtual ~AddMethod()
   {
   }

   virtual std::string name() const
   {
      return "add";
   }

   virtual std::string description() const
   {
      return "Add to numbers together.";
   }

   virtual JsonRpcError::SharedPtr call(Json::Value& json_request, Json::Value& json_result)
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

   static RpcMethod::SharedPtr create()
   {
      AddMethod* add = new AddMethod;

      return RpcMethod::SharedPtr(add);
   }
};

//-----------------------------------------------------------------------------------
// Free function example
//-----------------------------------------------------------------------------------
JsonRpcError::SharedPtr substract(Json::Value& json_request, Json::Value& json_result)
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

   static JsonRpcError::SharedPtr answer(Json::Value& json_request, Json::Value& json_result)
   {
      json_result = "Hello World!";
      return nullptr;
   }
};

//-----------------------------------------------------------------------------------
// Quit example
//-----------------------------------------------------------------------------------
struct StopServer
{
   static bool _quit;
   static std::condition_variable* _data_cond;

   static JsonRpcError::SharedPtr quit(Json::Value& json_request, Json::Value& json_result)
   {
      StopServer::_quit = true;
      StopServer::_data_cond->notify_all();

      json_result = "Stopping server";
      
      return nullptr;
   }

};

bool StopServer::_quit = false;
std::condition_variable* StopServer::_data_cond;

//-----------------------------------------------------------------------------------
// Setup the logger options
//-----------------------------------------------------------------------------------
void setup_logger(std::fstream& file_stream)
{
   StreamOutputHandler::SharedPtr cout_handler = StreamOutputHandler::create(std::cout);
   StreamOutputHandler::SharedPtr file_handler = StreamOutputHandler::create(file_stream);

   Logger& logger = Logger::get_logger();

   logger.level(Logger::All);
   logger.output_handlers().push_back(cout_handler);
   logger.output_handlers().push_back(file_handler);
}

//-----------------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------------
int main (int argc, char** argv)
{
   std::fstream fout("add-json-rpc-server.log", std::fstream::out | std::fstream::trunc);
   setup_logger(fout);

   LOG_START();

   Server::SharedPtr server = HttpServer::create();

   JsonRpcRequestListener::SharedPtr rl = JsonRpcRequestListener::create();

   rl->register_method("add", AddMethod::create()); 
   rl->register_method("substract", JsonRpcMethodWrapper::create(substract, "substract", "Substract two numbers")); 
   rl->register_method("answer", JsonRpcMethodWrapper::create(Echo::answer, "answer")); 
   rl->register_method("quit", JsonRpcMethodWrapper::create(StopServer::quit, "quit")); 

   std::mutex m;
   std::condition_variable data_cond;
   
   StopServer::_quit = false;
   StopServer::_data_cond = &data_cond;

   server->add_request_listener(9090, rl);

   server->start();

   LOG(Info) << "Server started successfully";

   while (true)
   {
      std::unique_lock<std::mutex> lk(m);

      data_cond.wait(lk, [&] { return StopServer::_quit; });

      lk.unlock();

      if (StopServer::_quit)
         {
         server->stop();
         break;
         }
   }

   LOG_END();
   return EXIT_SUCCESS;
}
