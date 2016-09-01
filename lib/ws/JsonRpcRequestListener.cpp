/*
 * JsonRpcRequestListener.cpp
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

#include <orion/ws/JsonRpcRequestListener.h>

#include <jsoncpp/json/json.h>

#include <orion/Logging.h>
#include <orion/ErrorMacros.h>
#include <orion/ws/JsonRpcMethod.h>

#define JSON_RPC_METHODNAME      "method"
#define JSON_RPC_LABEL           "jsonrpc"
#define JSON_RPC_ID              "id"
#define JSON_RPC_PARAMETERS      "params"
#define JSON_RPC_RESPONSE_ERROR  "error"
#define JSON_RPC_RESPONSE_RESULT "result"
#define JSON_RPC_VERSION         "2.0"

namespace orion
{
namespace ws
{
using namespace orion::net;
   
JsonRpcRequestListener::JsonRpcRequestListener(const std::string& uri) :
   RpcRequestListener(uri)
{
}

JsonRpcRequestListener::~JsonRpcRequestListener()
{
}

std::unique_ptr<Response> JsonRpcRequestListener::on_post(const Request* request)
{
   LOG_FUNCTION(Debug2, "JsonRpcRequestListener::on_post()")

   Json::Reader reader;
   Json::Value  json_request;
   Json::Value  json_response;
   Json::Value  json_result;

   if (reader.parse(request->content(), json_request, false))
   {
      if (json_request.isArray())
      {
         for (uint32_t i = 0; i < json_request.size(); i++)
         {
            Json::Value id = json_request[i][JSON_RPC_ID];

            auto error = process_method(json_request[i], json_result);

            json_response.append((not error) ? make_json_rpc_response(id, json_result) :
                                               make_json_rpc_response(id, error.get()));
         }
        
      } 
      else if (json_request.isObject())
      {
         Json::Value id = json_request[JSON_RPC_ID];

         auto error = process_method(json_request, json_result);

         json_response = (not error) ? make_json_rpc_response(id, json_result) :
                                       make_json_rpc_response(id, error.get());
      }
   }
   else
   {
      auto error = JsonRpcError::create_parse_error(request->content());
      json_response = make_json_rpc_response("", error.get());
   }
   
   Json::FastWriter writer;
   return Response::create(writer.write(json_response), "application/json");
}

//!
std::unique_ptr<JsonRpcError> JsonRpcRequestListener::process_method(Json::Value& json_request, Json::Value& json_result)
{
   LOG_FUNCTION(Debug2, "JsonRpcRequestListener::process_method()")

   std::unique_ptr<JsonRpcError> error = validate(json_request);

   if (error)
      return error;

   RpcMethod* method = get_method(json_request[JSON_RPC_METHODNAME].asString());

   if (not method)
      return JsonRpcError::create_method_not_found();

   JsonRpcMethod* json_method = dynamic_cast<JsonRpcMethod*>(method);

   return json_method->call(json_request, json_result);
}

//!
std::unique_ptr<JsonRpcError> JsonRpcRequestListener::validate(Json::Value& json_request)
{
   LOG_FUNCTION(Debug2, "JsonRpcRequestListener::validate()")

   // Validate request
   if (not json_request.isObject())
      return JsonRpcError::create_invalid_request();

   // Must be JSON-RPC version => 2.0 
   if (not json_request.isMember(JSON_RPC_LABEL) or 
           json_request[JSON_RPC_LABEL] != JSON_RPC_VERSION) 
      return JsonRpcError::create_invalid_request();
   
   // Must have method as string
   if (not json_request.isMember(JSON_RPC_METHODNAME) or 
       not json_request[JSON_RPC_METHODNAME].isString())
      return JsonRpcError::create_invalid_request();

   // Validate if id is present, is not its a notification
   if (json_request.isMember(JSON_RPC_ID) and
       (json_request[JSON_RPC_ID].isArray() or json_request[JSON_RPC_ID].isObject()))
      return JsonRpcError::create_invalid_request();
      
   return nullptr;
}

//!
Json::Value JsonRpcRequestListener::make_json_rpc_response(const Json::Value& id, Json::Value& json_result)
{
   Json::Value  json_response;

   json_response[JSON_RPC_LABEL] = JSON_RPC_VERSION;
   json_response[JSON_RPC_ID]    = id;

   json_response[JSON_RPC_RESPONSE_RESULT] = json_result;

   return json_response;
}

//!
Json::Value JsonRpcRequestListener::make_json_rpc_response(const Json::Value& id, const JsonRpcError* error)
{
   Json::Value  json_response;

   json_response[JSON_RPC_LABEL] = JSON_RPC_VERSION;
   json_response[JSON_RPC_ID]    = id;

   json_response[JSON_RPC_RESPONSE_ERROR] = error->to_json();

   return json_response;
}

std::unique_ptr<JsonRpcRequestListener> JsonRpcRequestListener::create(const std::string& uri)
{
   return std::make_unique<JsonRpcRequestListener>(uri);
}

} // ws
} // orion


