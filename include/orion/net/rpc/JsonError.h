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

#ifndef ORION_NET_RPC_JSONRPCERROR_H
#define ORION_NET_RPC_JSONRPCERROR_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/rpc/Error.h>

namespace orion
{
namespace net
{
namespace rpc
{

enum class JsonErrc
{
   ParseError      = -32700,
   InvalidRequest  = -32600,
   MethodNotFound  = -32601,
   InvalidParams   = -32602,
   InternalError   = -32603,
   ServerError     = -32000
};

API_EXPORT Error make_error(JsonErrc code) noexcept;

} // namespace rpc
} // namespace net
} // namespace orion
#endif // ORION_NET_RPC_JSONRPCERROR_H
