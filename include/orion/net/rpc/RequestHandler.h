/*
 * RequestHandler.h
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

#ifndef ORION_NET_RPC_REQUESTHANDLER_H
#define ORION_NET_RPC_REQUESTHANDLER_H

#include <map>
#include <memory>

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/RequestHandler.h>
#include <orion/net/rpc/Method.h>

namespace orion
{
namespace net
{
namespace rpc
{
using Methods = std::map<std::string, std::shared_ptr<Method>>;

/// Provides RPC protocol handler.
///
/// Using the RpcRequestListener class, you can create a simple RPC protocol handler that 
/// responds to RPC requests. 
///
class API_EXPORT RequestHandler : public http::RequestHandler
{
public:
   NO_COPY(RequestHandler)

   virtual ~RequestHandler();

   template <typename MethodHandler>
   void register_method(MethodHandler&& method)
   {
      auto m = std::make_shared<MethodHandler>(std::move(method));

      _methods.emplace(std::make_pair(m->name(), m));
   }

   std::weak_ptr<Method> get_method(const std::string& name) const;

protected:
   RequestHandler(const std::string& uri);

protected:
   Methods _methods;

};

} // rpc
} // net
} // orion

#endif // ORION_NET_RPC_REQUESTHANDLER_H
