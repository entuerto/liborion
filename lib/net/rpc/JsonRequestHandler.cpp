/*
 * JsonRequestHandler.cpp
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
#include <orion/net/rpc/JsonRequestHandler.h>

#include <jsoncpp/json/json.h>

#include <orion/Log.h>
#include <orion/ErrorMacros.h>

#define JSON_RPC_METHODNAME      "method"
#define JSON_RPC_LABEL           "jsonrpc"
#define JSON_RPC_ID              "id"
#define JSON_RPC_PARAMETERS      "params"
#define JSON_RPC_RESPONSE_ERROR  "error"
#define JSON_RPC_RESPONSE_RESULT "result"
#define JSON_RPC_VERSION         "2.0"

using namespace orion::log;

namespace orion
{
namespace net
{
namespace rpc
{
  
JsonRequestHandler::JsonRequestHandler(const std::string& uri) :
   RequestHandler(uri)
{
}

JsonRequestHandler::~JsonRequestHandler()
{
}

std::error_code JsonRequestHandler::on_post(const http::Request& request, http::Response& response)
{
   LOG_FUNCTION(Debug2, "JsonRequestHandler::on_post()")

   Json::Reader reader;
   Json::Value  json_request;
   Json::Value  json_response;
   Json::Value  json_result;

   std::ostringstream ostr;

   ostr << request.rdbuf();

   if (reader.parse(ostr.str(), json_request, false))
   {
      if (json_request.isArray())
      {
         for (uint32_t i = 0; i < json_request.size(); i++)
         {
            Json::Value id = json_request[i][JSON_RPC_ID];

            auto error = process_method(json_request[i], json_result);

            json_response.append((not error) ? make_response(id, json_result) :
                                               make_response(id, error));
         }
        
      } 
      else if (json_request.isObject())
      {
         Json::Value id = json_request[JSON_RPC_ID];

         auto error = process_method(json_request, json_result);

         json_response = (not error) ? make_response(id, json_result) :
                                       make_response(id, error);
      }
   }
   else
   {
      auto error = make_error(JsonErrc::ParseError);
      json_response = make_response("", error);
   }
   
   std::ostream o(response.rdbuf());

   response.header("Content-Type", "application/json");

   Json::FastWriter writer;

   o << writer.write(json_response);

   return std::error_code();
}

//!
Error JsonRequestHandler::process_method(Json::Value& json_request, Json::Value& json_result)
{
   LOG_FUNCTION(Debug2, "JsonRequestHandler::process_method()")

   auto error = validate(json_request);

   if (error)
      return error;

   auto method = get_method(json_request[JSON_RPC_METHODNAME].asString());

   if (method.expired())
      return make_error(JsonErrc::MethodNotFound);

   std::shared_ptr<JsonMethod> json_method;

   try
   {
      json_method = std::dynamic_pointer_cast<JsonMethod>(method.lock());
   }
   catch (const std::exception& e)
   {
      log::exception(e);

      return make_error(JsonErrc::MethodNotFound);
   }

   return (*json_method)(json_request, json_result);
}

//!
Error JsonRequestHandler::validate(Json::Value& json_request)
{
   LOG_FUNCTION(Debug2, "JsonRequestHandler::validate()")

   // Validate request
   if (not json_request.isObject())
      return make_error(JsonErrc::InvalidRequest);

   // Must be JSON-RPC version => 2.0 
   if (not json_request.isMember(JSON_RPC_LABEL) or 
           json_request[JSON_RPC_LABEL] != JSON_RPC_VERSION) 
      return make_error(JsonErrc::InvalidRequest);
   
   // Must have method as string
   if (not json_request.isMember(JSON_RPC_METHODNAME) or 
       not json_request[JSON_RPC_METHODNAME].isString())
      return make_error(JsonErrc::InvalidRequest);

   // Validate if id is present, is not its a notification
   if (json_request.isMember(JSON_RPC_ID) and
       (json_request[JSON_RPC_ID].isArray() or json_request[JSON_RPC_ID].isObject()))
      return make_error(JsonErrc::InvalidRequest);
      
   return Error();
}

//!
Json::Value make_response(const Json::Value& id, Json::Value& json_result)
{
   Json::Value  json_response;

   json_response[JSON_RPC_LABEL] = JSON_RPC_VERSION;
   json_response[JSON_RPC_ID]    = id;

   json_response[JSON_RPC_RESPONSE_RESULT] = json_result;

   return json_response;
}

//!
Json::Value make_response(const Json::Value& id, const Error& error)
{
   Json::Value  json_response;

   json_response[JSON_RPC_LABEL] = JSON_RPC_VERSION;
   json_response[JSON_RPC_ID]    = id;

   Json::Value msg;

   msg["code"]    = error.code();
   msg["message"] = error.message();
   msg["data"]    = error.data();

   json_response[JSON_RPC_RESPONSE_ERROR] = msg;

   return json_response;
}

} // rpc
} // net
} // orion
