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

#include <orion/net/rpc/Types.h>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace orion
{
namespace net
{
namespace rpc
{
template<typename Derived, typename T>
class Method;
///
/// Class to define RPC methods
///
template<typename Derived, typename Ret, typename... ArgTs>
class Method<Derived, Ret(ArgTs...)>
{
public:
   /// User defined function type.
   using Type = Ret(ArgTs...);
   /// Return type.
   using ReturnType = Ret;

   static std::string prototype()
   {
      static std::string p = fmt::format(
         "{} {}({})", TypeName<ReturnType>::name(), Derived::name(), TypeNameSequence<ArgTs...>());
      return p;
   }
};

struct MethodInfo
{
   std::string name;
   std::string description;
   Version version;
   std::string prototype;
};

} // namespace rpc
} // namespace net
} // namespace orion

#endif // ORION_NET_RPC_METHOD_H
