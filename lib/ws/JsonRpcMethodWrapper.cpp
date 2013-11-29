/*
 * JsonRpcMethod.cpp
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

#include <orion/ws/JsonRpcMethodWrapper.h>

namespace orion
{
namespace ws
{

JsonRpcMethodWrapper::JsonRpcMethodWrapper(      std::function<JsonRpcError::SharedPtr(Json::Value&, Json::Value&)> func, 
                                           const std::string& name, 
                                           const std::string& desc) :
   JsonRpcMethod(),
   _function(func),
   _name(name),
   _description(desc)
{
}

JsonRpcMethodWrapper::~JsonRpcMethodWrapper()
{
}

std::string JsonRpcMethodWrapper::name() const
{
   return _name;
}

std::string JsonRpcMethodWrapper::description() const
{
   return _description;
}

JsonRpcError::SharedPtr JsonRpcMethodWrapper::call(Json::Value& json_request, Json::Value& json_result)
{
   return _function(json_request, json_result);
}

RpcMethod::SharedPtr JsonRpcMethodWrapper::create(      std::function<JsonRpcError::SharedPtr(Json::Value&, Json::Value&)> func, 
                                                  const std::string& name, 
                                                  const std::string& desc)
{
   JsonRpcMethodWrapper* wrapper = new JsonRpcMethodWrapper(func, name, desc);
   return RpcMethod::SharedPtr(wrapper);
}

} // ws
} // orion

