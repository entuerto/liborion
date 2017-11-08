//
// Method.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_METHOD_H
#define ORION_NET_RPC_METHOD_H

#include <orion/Orion-Stddefs.h>

namespace orion
{
namespace net
{
namespace rpc
{
template <typename T>
class Method;
///
/// Class to define RPC methods
///
template <typename Ret, typename... Args>
class Method<Ret(Args...)>
{
public:
   /// User defined function type.
   using Type = Ret(Args...);
   /// Return type.
   using ReturnType = Ret;
};

} // namespace rpc
} // namespace net
} // namespace orion

#endif // ORION_NET_RPC_METHOD_H
