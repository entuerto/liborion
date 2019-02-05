//
// Serializer.h
//
// Copyright (c) 2013-2018 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_SERIALIZER_H
#define ORION_NET_RPC_SERIALIZER_H

#include <orion/Common.h>

#include <orion/net/rpc/Error.h>

#include <tuple>
#include <utility>

namespace orion
{
namespace net
{
namespace rpc
{

template<typename Protocol>
class Serializer
{
public:
   using RequestType  = typename Protocol::Request;
   using ResponseType = typename Protocol::Response;

   template<typename ArgT>
   static Error serialize(RequestType& req, ArgT&& arg)
   {
      return Serializer<Protocol>::serialize(req, std::forward<ArgT>(arg));
   }

   template<typename ArgT>
   static Error deserialize(ResponseType& res, ArgT& arg)
   {
      return Serializer<Protocol>::deserialize(res, arg);
   }
};

template<typename Protocol>
class ArgsSerializer
{
public:
   using CodecType    = typename Protocol::CodecType;
   using RequestType  = typename Protocol::Request;
   using ResponseType = typename Protocol::Response;

   static Error serialize(RequestType& req) { return Error{}; }

   template<typename ProtoArgT>
   static Error serialize(RequestType& req, ProtoArgT&& proto_arg)
   {
      return Serializer<Protocol>::serialize(req.params, std::forward<ProtoArgT>(proto_arg));
   }

   template<typename ProtoArgT, typename... ProtoArgTs>
   static Error serialize(RequestType& req, ProtoArgT&& proto_arg, ProtoArgTs&&... proto_args)
   {
      if (auto err = ArgsSerializer<Protocol>::serialize(req, std::forward<ProtoArgT>(proto_arg)))
         return err;

      if (auto err = CodecType::emit_separator(req.params))
         return err;

      return ArgsSerializer<Protocol>::serialize(req, std::forward<ProtoArgTs>(proto_args)...);
   }

   static Error deserialize(RequestType& req) { return Error{}; }

   template<typename ProtoArgT>
   static Error deserialize(RequestType& req, ProtoArgT& proto_arg)
   {
      return Serializer<Protocol>::deserialize(req.params, proto_arg);
   }

   template<typename ProtoArgT, typename... ProtoArgTs>
   static Error deserialize(RequestType& req, ProtoArgT& proto_arg, ProtoArgTs&... proto_args)
   {
      if (auto err = ArgsSerializer<Protocol>::deserialize(req, proto_arg))
         return err;

      if (auto err = CodecType::consume_separator(req.params))
         return err;

      return ArgsSerializer<Protocol>::deserialize(req, proto_args...);
   }
};

template<typename Protocol>
class MethodSerializer
{
public:
   using CodecType    = typename Protocol::CodecType;
   using RequestType  = typename Protocol::Request;
   using ResponseType = typename Protocol::Response;

   template<typename MethodT, typename... ArgTs>
   static Error serialize(RequestType& req, const ArgTs... args)
   {
      // Method name
      req.method = MethodT::name();

      // Method parameters
      return ArgsSerializer<Protocol>::serialize(req, args...);
   }

};

} // namespace rpc
} // namespace net
} // namespace orion

#endif // ORION_NET_RPC_SERIALIZER_H
