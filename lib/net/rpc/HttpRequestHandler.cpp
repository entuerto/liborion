//
// HttpRequestHandler.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

//#include <net/rpc/HttpRequestHandler.h>

#include <orion/ErrorMacros.h>
#include <orion/Log.h>
#include <orion/net/rpc/Error.h>

#include <net/rpc/JsonError.h>

#include <json/json.hpp>

#include <iostream>

#define JSON_RPC_METHODNAME "method"
#define JSON_RPC_PARAMETERS "params"
#define JSON_RPC_RESPONSE_RESULT "result"

using json = nlohmann::json;

namespace orion
{
namespace net
{
namespace rpc
{
//-------------------------------------------------------------------------------------------------
#if 0
static Json::Value make_response(Json::Value& json_result);

static Json::Value make_response(const Error& error);
#endif
//-------------------------------------------------------------------------------------------------

//HttpRequestHandler::HttpRequestHandler(std::unique_ptr<Service>&& service)
//   : _service(std::move(service))
//{
//}

#if 0

std::error_code HttpRequestHandler::on_post(const http::Request& request, http::Response& response)
{
   LOG_FUNCTION(Debug2, "HttpRequestHandler::on_post()")

   if (request.header("Content-Type") == "application/json")
   {
      // deserialize from standard input
      json j;
      std::iostream(request.rdbuf()) >> j;

      // the setw manipulator was overloaded to set the indentation for pretty printing
      std::cout << std::setw(4) << j << "\n";
   }
   else if (request.header("Content-Type") == "application/json+cbor")
   {
   }
   else if (request.header("Content-Type") == "application/cbor")
   {
   }

   return std::error_code();
}

//!
Json::Value make_response(Json::Value& json_result)
{
   Json::Value  json_response;

   json_response[JSON_RPC_RESPONSE_RESULT] = json_result;

   return json_response;
}

//!
Json::Value make_response(const Error& error)
{
   Json::Value  json_response;

   Json::Value msg;

   msg["code"]    = error.code();
   msg["message"] = error.message();
   msg["data"]    = error.data();

   json_response[JSON_RPC_RESPONSE_RESULT] = msg;

   return json_response;
}
#endif
} // namespace rpc
} // namespace net
} // namespace orion
