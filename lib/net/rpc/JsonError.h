//
// JsonError.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_JSONERROR_H
#define ORION_NET_RPC_JSONERROR_H

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
   ParseError     = -32700,
   InvalidRequest = -32600,
   MethodNotFound = -32601,
   InvalidParams  = -32602,
   InternalError  = -32603,
   ServerError    = -32000
};

API_EXPORT Error make_error(JsonErrc code) noexcept;

} // namespace rpc
} // namespace net
} // namespace orion
#endif // ORION_NET_RPC_JSONERROR_H
