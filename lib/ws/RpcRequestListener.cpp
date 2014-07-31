/*
 * RpcRequestListener.cpp
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

#include <orion/ws/RpcRequestListener.h>

namespace orion
{
namespace ws
{
RpcRequestListener::RpcRequestListener(const std::string& uri) :
   RequestListener(uri),
   _RpcMethods()
{
}

RpcRequestListener::~RpcRequestListener()
{
}

void RpcRequestListener::register_method(const std::string& name, RpcMethod::SharedPtr method)
{
   _RpcMethods.insert(std::pair<std::string, RpcMethod::SharedPtr>(name, method));
}

RpcMethod::SharedPtr RpcRequestListener::get_method(const std::string& name)
{
   auto it = _RpcMethods.find(name);

   if (it != _RpcMethods.end())
   {
      return it->second;
   }

   return nullptr;
}

Response::SharedPtr RpcRequestListener::on_get(Request::SharedPtr request)
{
   return Response::create_404();
}

Response::SharedPtr RpcRequestListener::on_post(Request::SharedPtr request)
{
   return Response::create_404();
}

Response::SharedPtr RpcRequestListener::on_put(Request::SharedPtr request)
{
   return Response::create_404();
}

Response::SharedPtr RpcRequestListener::on_delete(Request::SharedPtr request)
{
   return Response::create_404();
}

} // ws
} // orion

