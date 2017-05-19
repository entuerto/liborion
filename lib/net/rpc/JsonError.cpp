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

#include <net/rpc/JsonError.h>

#include <jsoncpp/json/json.h>

namespace orion
{
namespace net
{
namespace rpc
{

static const std::map<JsonErrc, std::string> JsonErrorText{
   {JsonErrc::ParseError, "An error occurred on the server while parsing the JSON text."},
   {JsonErrc::InvalidRequest, "The JSON sent is not a valid Request object."},
   {JsonErrc::MethodNotFound, "The method does not exist / is not available."},
   {JsonErrc::InvalidParams, "Invalid method parameter(s)."},
   {JsonErrc::InternalError, "Internal JSON-RPC error."}};

Error make_error(JsonErrc code) noexcept
{
   auto text = JsonErrorText.find(code);
   if (text != JsonErrorText.end())
      return Error(static_cast<int32_t>(code), text->second);

   return Error(static_cast<int32_t>(code));
}

} // namespace rpc
} // namespace net
} // namespace orion
