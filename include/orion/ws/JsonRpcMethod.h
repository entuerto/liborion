/*
 * JsonRpcMethod.h
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

#ifndef ORION_WS_JSONRPCMETHOD_H
#define ORION_WS_JSONRPCMETHOD_H

#include <memory>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/ws/RpcMethod.h>

namespace orion
{
namespace ws
{
//! Abstract class to define JSON-RPC methods
/*!
   
 */
class JsonRpcMethod : public RpcMethod
{
public:
   virtual ~JsonRpcMethod() = default;

   virtual std::unique_ptr<JsonRpcError> call(Json::Value& json_request, Json::Value& json_result) =0;
};

} // ws
} // orion

#endif

