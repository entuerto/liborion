/*
 * JsonRpcError.h
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

#ifndef ORION_WS_JSONRPCERROR_H
#define ORION_WS_JSONRPCERROR_H

#include <string>
#include <orion/MemoryUtils.h>
#include <orion/ws/RpcError.h>

// Forward declares
namespace Json
{
class Value;
}

namespace orion
{
namespace ws
{
//! Abstract class to define RPC errors
/*!
   
 */
class JsonRpcError : public RpcError
{
public:
   DECLARE_POINTERS(JsonRpcError)

   virtual ~JsonRpcError();

   Json::Value to_json() const;

    //! Create a RPC Error object
   static JsonRpcError::SharedPtr create(int32_t code, const std::string& message, const std::string& data = "");

   //! Invalid JSON was received by the server.
   static JsonRpcError::SharedPtr create_parse_error(const std::string& data = "");

   //! The JSON sent is not a valid Request object.
   static JsonRpcError::SharedPtr create_invalid_request(const std::string& data = "");

   //! The method does not exist / is not available.
   static JsonRpcError::SharedPtr create_method_not_found(const std::string& data = "");

   //! Invalid method parameter(s).
   static JsonRpcError::SharedPtr create_invalid_params(const std::string& data = "");

   //! Internal JSON-RPC error.
   static JsonRpcError::SharedPtr create_internal_error(const std::string& data = "");


protected:
  JsonRpcError(int32_t code, const std::string& message, const std::string& data);

};

} // ws
} // orion

#endif

