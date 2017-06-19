/*
 * HttpHttpRequestHandler.cpp
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <net/rpc/HttpRequestHandler.h>

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

HttpRequestHandler::HttpRequestHandler(std::unique_ptr<Service>&& service)
   : http::RequestHandler(service->url())
   , _service(std::move(service))
{
}

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

#if 0
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
