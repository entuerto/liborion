//
// Traits.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_TRAITS_H
#define ORION_NET_RPC_TRAITS_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/rpc/Error.h>

#include <tuple>
#include <utility>

namespace orion
{
namespace net
{
namespace rpc
{
//-------------------------------------------------------------------------------------------------
// MethodArgsTuple

/// Provides a typedef for a tuple containing the decayed argument types.
template <typename T> 
class MethodArgsTuple;

template <typename Ret, typename... Args>
class MethodArgsTuple<Ret(Args...)> 
{
public:
  using Type = std::tuple<typename std::decay<
      typename std::remove_reference<Args>::type>::type...>;
};

//-------------------------------------------------------------------------------------------------
// ReadArgs

// Helper that provides a Functor for deserializing arguments.
template <typename... Args> 
class ReadArgs 
{
public:
   Error operator()() 
   { 
      return Error(0); 
      //return Error::success(); 
   }
};

template <typename Arg, typename... Args>
class ReadArgs<Arg, Args...> : public ReadArgs<Args...> 
{
public:
   ReadArgs(Arg& arg, Args&... args)
      : ReadArgs<Args...>(args...), _arg(arg) {}

   Error operator()(Arg& arg_val, Args&... arg_vals) 
   {
      this->_arg = std::move(arg_val);
      return ReadArgs<Args...>::operator()(arg_vals...);
   }

private:
   Arg& _arg;
};

//-------------------------------------------------------------------------------------------------
// ArgumentIndexPack

template <std::size_t... ArgumentIndexes>
struct ArgumentIndexPack;

//-------------------------------------------------------------------------------------------------
// GenerateArgumentIndexPack

template <std::size_t NumberOfArgumentIndexesToGenerate, std::size_t... GeneratedArgumentIndexes>
struct GenerateArgumentIndexPack 
   : GenerateArgumentIndexPack<NumberOfArgumentIndexesToGenerate - 1, 
                               NumberOfArgumentIndexesToGenerate - 1, 
                               GeneratedArgumentIndexes...>{};

template <std::size_t... GeneratedArgumentIndexes>
struct GenerateArgumentIndexPack<0, GeneratedArgumentIndexes...>
{
   using Pack = ArgumentIndexPack<GeneratedArgumentIndexes...>;
};

//-------------------------------------------------------------------------------------------------

template <std::size_t N, std::size_t... I>
struct build_index_impl : build_index_impl<N - 1, N - 1, I...> {};

template <std::size_t... I>
struct build_index_impl<0, I...> : std::index_sequence<I...> {};

/// Creates a compile-time integer sequence for a parameter pack.
template <class... Ts>
struct index_sequence_for : build_index_impl<sizeof...(Ts)> {};

//-------------------------------------------------------------------------------------------------

// The base case applies to non-function types (the template class is
// specialized for function types) and inherits from the appropriate
// specialization for the given non-function type's call operator.
template <typename HandlerT>
class HandlerTraits 
   : public HandlerTraits<decltype(&std::remove_reference<HandlerT>::type::operator())> {};

// Traits for handlers with a given function type.
template <typename RetT, typename... ArgTs>
class HandlerTraits<RetT(ArgTs...)> 
{
public:
   // Function type of the handler.
   using Type = RetT(ArgTs...);

   // Return type of the handler.
   using ReturnType = RetT;

   // Call the given handler with the given arguments.
   template <typename HandlerT, typename... TArgTs>
   //static typename WrappedHandlerReturn<RetT>::Type
   static ReturnType
   unpack_and_invoke(HandlerT& handler, std::tuple<TArgTs...>& args) 
   {
      return unpack_and_invoke_impl(handler, args, index_sequence_for<TArgTs...>());
   }

   // Call the given handler with the given arguments.
   template <typename HandlerT>
   static typename std::enable_if<
      std::is_void<typename HandlerTraits<HandlerT>::ReturnType>::value,
      Error>::type
   invoke(HandlerT& handler, ArgTs&&... args) 
   {
      handler(std::move(args)...);
      return Error();
   }

   template <typename HandlerT, typename... TArgTs>
   static typename std::enable_if<
      !std::is_void<typename HandlerTraits<HandlerT>::ReturnType>::value,
      typename HandlerTraits<HandlerT>::ReturnType>::type
   invoke(HandlerT& handler, TArgTs... args) 
   {
      return handler(std::move(args)...);
   }

   // Serialize arguments to the stream.
   template <typename StreamT, typename... StreamArgTs>
   static Error serialize_args(StreamT& s, const StreamArgTs... stream_args) 
   {
      //return SequenceSerialization<StreamT, ArgTs...>::serialize(s, stream_args...);
      return Error();
   }

   // Deserialize arguments from the stream.
   template <typename StreamT, typename... StreamArgTs>
   static Error deserialize_args(StreamT& s, std::tuple<StreamArgTs...>& args) 
   {
      return deserialize_args_impl(s, args, index_sequence_for<StreamArgTs...>());
   }

private:
   template <typename StreamT, typename... StreamArgTs, size_t... Indexes>
   static Error deserialize_args_impl(StreamT& s, std::tuple<StreamArgTs...>& args, std::index_sequence<Indexes...> _) 
   {
      //return SequenceSerialization<StreamT, ArgTs...>::deserialize(s, std::get<Indexes>(args)...);
      return Error();
   }

   template <typename HandlerT, typename ArgTuple, size_t... Indexes>
   //static typename WrappedHandlerReturn<typename HandlerTraits<HandlerT>::ReturnType>::Type
   static ReturnType
   unpack_and_invoke_impl(HandlerT& handler, ArgTuple& args, std::index_sequence<Indexes...>) 
   {
      return invoke(handler, std::move(std::get<Indexes>(args))...);
   }
};

// Handler traits for free functions.
template <typename RetT, typename... ArgTs>
class HandlerTraits<RetT(*)(ArgTs...)>
   : public HandlerTraits<RetT(ArgTs...)> {};

// Handler traits for class methods (especially call operators for lambdas).
template <typename Class, typename RetT, typename... ArgTs>
class HandlerTraits<RetT (Class::*)(ArgTs...)>
   : public HandlerTraits<RetT(ArgTs...)> {};

// Handler traits for const class methods (especially call operators for
// lambdas).
template <typename Class, typename RetT, typename... ArgTs>
class HandlerTraits<RetT (Class::*)(ArgTs...) const>
   : public HandlerTraits<RetT(ArgTs...)> {};

//-------------------------------------------------------------------------------------------------

// Helper for wrapping member functions up as functors. 
template <typename Class, typename Ret, typename... Args>
class MemberFnWrapper 
{
public:
   using MethodType = Ret (Class::*)(Args...);

   MemberFnWrapper(Class &instance, MethodType method)
      : _instance(instance)
      , _method(method) {}

   Ret operator()(Args&&... args) 
   {
      return (_instance.*_method)(std::move(args)...);
   }

private:
   Class& _instance;
   MethodType _method;
};

//-------------------------------------------------------------------------------------------------

template <typename Handler, typename ArgsTuple, size_t ... Is>
inline auto invoke_handler_impl(Handler&& h, ArgsTuple&& args_tuple, std::index_sequence<Is...>)
{
   using ArgsTupleType = std::remove_reference_t<ArgsTuple>;

   return h(std::forward<std::tuple_element_t<Is, ArgsTupleType>>(std::get<Is>(args_tuple))...);
}

template <typename Handler, typename ArgsTuple>
inline auto invoke_handler(Handler&& h, ArgsTuple&& args_tuple)
{
   using IndicesType = std::make_index_sequence<std::tuple_size<std::remove_reference_t<ArgsTuple>>::value>;

   return invoke_handler_impl(std::forward<Handler>(h), std::forward<ArgsTuple>(args_tuple), IndicesType{});
}

} // namespace rpc
} // namespace net
} // namespace orion

#endif // ORION_NET_RPC_TRAITS_H
