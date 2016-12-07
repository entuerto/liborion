/*
 * RequestHandler.cpp
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

#include <orion/net/rpc/RequestHandler.h>

namespace orion
{
namespace net
{
namespace rpc
{

RequestHandler::RequestHandler(const std::string& uri) :
   http::RequestHandler(uri),
   _methods()
{
}

RequestHandler::~RequestHandler()
{
}

std::weak_ptr<Method> RequestHandler::get_method(const std::string& name) const
{
   const auto& it = _methods.find(name);

   if (it != _methods.end())
   {
      return it->second;
   }

   return std::weak_ptr<Method>();
}

} // namespace rpc
} // namespace net
} // namespace orion
