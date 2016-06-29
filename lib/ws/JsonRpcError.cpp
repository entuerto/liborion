/*
 * JsonRpcError.cpp
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

#include <orion/ws/JsonRpcError.h>

#include <jsoncpp/json/json.h>

namespace orion
{
namespace ws
{
//!
JsonRpcError::JsonRpcError(int32_t code, const std::string& message, const std::string& data) :
   RpcError(code, message, data)
{

}

//!
JsonRpcError::~JsonRpcError()
{
}

//!
Json::Value JsonRpcError::to_json() const
{
   Json::Value error;

   error["code"] = code();
   error["message"] = message();
   error["data"] = data();

   return error;
}

//! Create a RPC Error object
JsonRpcError::SharedPtr JsonRpcError::create(int32_t code, const std::string& message, const std::string& data)
{
   return JsonRpcError::SharedPtr(new JsonRpcError(code, message, data));
}

//! Invalid JSON was received by the server.
JsonRpcError::SharedPtr JsonRpcError::create_parse_error(const std::string& data)
{
   return create(-32700, "An error occurred on the server while parsing the JSON text.", data);
}

//! The JSON sent is not a valid Request object.
JsonRpcError::SharedPtr JsonRpcError::create_invalid_request(const std::string& data)
{
   return create(-32600, "The JSON sent is not a valid Request object.", data);
}

//! The method does not exist / is not available.
JsonRpcError::SharedPtr JsonRpcError::create_method_not_found(const std::string& data)
{
   return create(-32601, "The method does not exist / is not available.", data);
}

//! Invalid method parameter(s).
JsonRpcError::SharedPtr JsonRpcError::create_invalid_params(const std::string& data)
{
   return create(-32602, "Invalid method parameter(s).", data);
}

//! Internal JSON-RPC error.
JsonRpcError::SharedPtr JsonRpcError::create_internal_error(const std::string& data)
{
   return create(-32603, "Internal JSON-RPC error.", data);
}

} // ws
} // orion

