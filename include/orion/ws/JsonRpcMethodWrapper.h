/*
 * JsonRpcMethodWrapper.h
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

#ifndef ORION_WS_JSONRPCMETHODWRAPPER_H
#define ORION_WS_JSONRPCMETHODWRAPPER_H

#include <functional>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/ws/JsonRpcError.h>
#include <orion/ws/JsonRpcMethod.h>

namespace orion
{
namespace ws
{
//! Abstract class to define JSON-RPC methods
/*!
   
 */
class API_EXPORT JsonRpcMethodWrapper : public JsonRpcMethod
{
public:
   virtual ~JsonRpcMethodWrapper();

   virtual std::string name() const override;

   virtual std::string description() const override;

   virtual std::unique_ptr<JsonRpcError> call(Json::Value& json_request, Json::Value& json_result) override;

   static std::unique_ptr<RpcMethod> create(      std::function<std::unique_ptr<JsonRpcError>(Json::Value&, Json::Value&)> func, 
                                            const std::string& name, 
                                            const std::string& desc = "");

public:
   JsonRpcMethodWrapper(      std::function<std::unique_ptr<JsonRpcError>(Json::Value&, Json::Value&)> func, 
                        const std::string& name, 
                        const std::string& desc = "");

private:
   std::string _name;
   std::string _description;
   std::function<std::unique_ptr<JsonRpcError>(Json::Value&, Json::Value&)> _function;
};

} // ws
} // orion

#endif

