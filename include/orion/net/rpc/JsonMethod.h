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

#ifndef ORION_NET_RPC_JSONMETHOD_H
#define ORION_NET_RPC_JSONMETHOD_H

#include <functional>
#include <memory>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/net/rpc/Error.h>
#include <orion/net/rpc/Method.h>

#include <jsoncpp/json/json.h>

namespace orion
{
namespace net
{
namespace rpc
{
using JsonFunctionHandler = std::function<Error(Json::Value&, Json::Value&)>;
///
/// Class to define JSON-RPC methods
///
class API_EXPORT JsonMethod : public Method
{
public:
   JsonMethod(const std::string& name, const std::string& desc);

   JsonMethod(const std::string& name, const std::string& desc, JsonFunctionHandler func);

   JsonMethod(const JsonMethod& rhs);
   JsonMethod(JsonMethod&& rhs);

   virtual ~JsonMethod() = default;

   virtual Error operator()(Json::Value& json_request, Json::Value& json_result);

   JsonMethod& operator=(const JsonMethod& rhs);
   JsonMethod& operator=(JsonMethod&& rhs);


private:
   JsonFunctionHandler _func;
};

} // rpc
} // net
} // orion

#endif // ORION_NET_RPC_JSONMETHOD_H
