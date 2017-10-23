//
// Service.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

#ifndef ORION_NET_RPC_SERVICE_H
#define ORION_NET_RPC_SERVICE_H

#include <orion/Orion-Stddefs.h>

#include <orion/Utils.h>
#include <orion/net/rpc/Method.h>
#include <orion/net/rpc/Error.h>
#include <orion/net/rpc/Result.h>
#include <orion/net/rpc/Traits.h>

#include <functional>
#include <map>
#include <memory>
#include <system_error>

namespace orion
{
namespace net
{
namespace rpc
{
///
/// Provides RPC service.
///
template <typename StreamT>
class Service
{
public:
   using MethodHandlerFunc = std::function<Error(StreamT&)>;
   using Methods = std::map<std::string, MethodHandlerFunc>;

   Service(const std::string& name, const Version& version);
   Service(const std::string& name, const Version& version, const std::string& desc);
   ~Service() = default;

   std::string name() const;

   std::string description() const;
   
   Version version() const;

   std::string url() const;

   /// Add a method for the given RPC service.
   /// This installs the given method functor for the given RPC service, and
   /// makes the RPC method available for negotiation/calling from the remote.
   template <typename MethodT, typename HandlerT>
   void register_method(HandlerT handler) 
   {
      register_method_impl<MethodT>(std::move(handler));
   }

   /// Add a class-method as a handler.
   template <typename MethodT, typename ClassT, typename RetT, typename... Args>
   void register_method(ClassT &object, RetT (ClassT::*method)(Args...)) 
   {
      register_method<MethodT>(MemberFnWrapper<ClassT, RetT, Args...>(object, method));
   }

   /// Remove the handler for the given method.
   /// A handler must currently be registered for this function.
   template <typename MethodT>
   void remove_method() 
   {
      auto it = _methods.find(MethodT::name());

      if (it != _methods.end())
         _methods.erase(it);
  }

   /// Clear all methods.
   void clear_methods() 
   {
      _methods.clear();
   }

   //std::weak_ptr<Method> get_method(const std::string& name) const;

   //std::error_code invoke_method(const std::string& name, const Params& params, Result& result);

protected:
   template <typename MethodT, typename HandlerT>
   void register_method_impl(HandlerT handler) 
   {

      //static_assert(detail::RPCArgTypeCheck<
      //                CanDeserializeCheck, typename Func::Type,
      //                typename detail::HandlerTraits<HandlerT>::Type>::value,
      //            "");

     _methods[MethodT::name()] = wrap_handler<MethodT>(std::move(handler));
   }

   // Wrap the given user handler in the necessary argument-deserialization code,
   // result-serialization code, and call to the launch policy (if present).
   template <typename MethodT, typename HandlerT>
   MethodHandlerFunc wrap_handler(HandlerT handler) 
   {
      return [this, handler](StreamT& stream) mutable -> Error 
      {
         // Start by deserializing the arguments.
         using ArgsTuple =
            typename MethodArgsTuple<
               typename HandlerTraits<HandlerT>::Type>::Type;

         auto args = std::make_shared<ArgsTuple>();
/*
      if (auto Err =
              detail::HandlerTraits<typename Func::Type>::deserializeArgs(
                  Channel, *Args))
        return Err;

      // GCC 4.7 and 4.8 incorrectly issue a -Wunused-but-set-variable warning
      // for RPCArgs. Void cast RPCArgs to work around this for now.
      // FIXME: Remove this workaround once we can assume a working GCC version.
      (void)args;

      // End receieve message, unlocking the channel for reading.
      if (auto Err = Channel.endReceiveMessage())
        return Err;

      using HTraits = detail::HandlerTraits<HandlerT>;
      using FuncReturn = typename Func::ReturnType;
      return detail::respond<FuncReturn>(Channel, ResponseId, SeqNo,
                                         HTraits::unpackAndRun(Handler, *Args));
*/
         return Error();
      };
   }
private:
   std::string _name;
   std::string _description;
   Version _version;

   Methods _methods;
};

//-------------------------------------------------------------------------------------------------
template <typename StreamT>
Service<StreamT>::Service(const std::string& name,
                          const Version& version)
   : _name(name)
   , _description()
   , _version(version)
   , _methods() {}

template <typename StreamT>
Service<StreamT>::Service(const std::string& name,
                          const Version& version,
                          const std::string& desc)
   : _name(name)
   , _description(desc)
   , _version(version)
   , _methods() {}

template <typename StreamT>
std::string Service<StreamT>::name() const
{
   return _name;
}

template <typename StreamT>
std::string Service<StreamT>::description() const
{
   return _description;
}

template <typename StreamT>
Version Service<StreamT>::version() const
{
   return _version;
}

} // rpc
} // net
} // orion

#endif // ORION_NET_RPC_SERVICE_H
