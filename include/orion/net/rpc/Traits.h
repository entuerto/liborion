//
// Traits.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_TRAITS_H
#define ORION_NET_RPC_TRAITS_H

#include <orion/Common.h>

#include <orion/net/rpc/Error.h>
#include <orion/net/rpc/Serializer.h>

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

// Checks that predicate P holds for each corresponding pair of type arguments
// from T1 and T2 tuple.
template<template<class, class> class P, typename T1Tuple, typename T2Tuple>
class ArgTypeCheckHelper;

template<template<class, class> class P>
class ArgTypeCheckHelper<P, std::tuple<>, std::tuple<>>
{
public:
   static const bool value = true;
};

template<template<class, class> class P, typename T, typename... Ts, typename U, typename... Us>
class ArgTypeCheckHelper<P, std::tuple<T, Ts...>, std::tuple<U, Us...>>
{
public:
   static const bool value =
      P<T, U>::value and ArgTypeCheckHelper<P, std::tuple<Ts...>, std::tuple<Us...>>::value;
};

template<template<class, class> class P, typename T1Sig, typename T2Sig>
class ArgTypeCheck
{
public:
   using T1Tuple = typename MethodArgsTuple<T1Sig>::Type;
   using T2Tuple = typename MethodArgsTuple<T2Sig>::Type;

   static_assert(std::tuple_size<T1Tuple>::value >= std::tuple_size<T2Tuple>::value,
                 "Too many arguments to RPC call");
   static_assert(std::tuple_size<T1Tuple>::value <= std::tuple_size<T2Tuple>::value,
                 "Too few arguments to RPC call");

   static const bool value = ArgTypeCheckHelper<P, T1Tuple, T2Tuple>::value;
};

template<typename Protocol, typename Type1>
class CanSerialize
{
private:
   using S = Serializer<Protocol>;
   using R = typename Protocol::Request;

   template<typename T>
   static std::true_type check(
      typename std::enable_if<std::is_same<decltype(S::serialize(std::declval<R&>(),
                                                                 std::declval<const Type1&>())),
                                           Error>::value,
                              void*>::type);

   template<typename>
   static std::false_type check(...);

public:
   static const bool value = decltype(check<S>(0))::value;
};

template<typename Protocol, typename Type1>
class CanDeserialize
{
private:
   using S = Serializer<Protocol>;
   using R = typename Protocol::Response;

   template<typename T>
   static std::true_type check(
      typename std::enable_if<
         std::is_same<decltype(S::deserialize(std::declval<R&>(), std::declval<Type1&>())),
                      Error>::value,
         void*>::type);

   template<typename>
   static std::false_type check(...);

public:
   static const bool value = decltype(check<S>(0))::value;
};

template<typename Protocol, typename T>
class TypeSerializerCheck : CanSerialize<Protocol, T>
{
public:
   using CanSerialize<Protocol, T>::value;

   static_assert(value, "Missing serializer for Type");
};

template<typename Protocol, typename T>
class TypeDeserializerCheck : CanDeserialize<Protocol, T>
{
public:
   using CanDeserialize<Protocol, T>::value;

   static_assert(value, "Missing deserializer for type");
};

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
   static ReturnType unpack_and_invoke(HandlerT& handler, std::tuple<TArgTs...>& args) 
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

   // Serialize arguments.
   template <typename Protocol, typename... ProtocolArgTs>
   static Error serialize_args(Protocol& p, const ProtocolArgTs... proto_args) 
   {
      return ArgsSerializer<Protocol>::serialize(p, proto_args...);
   }

   // Deserialize arguments.
   template <typename Protocol, typename Msg, typename... MArgTs>
   static Error deserialize_args(Msg& msg, std::tuple<MArgTs...>& args) 
   {
      return deserialize_args_impl<Protocol>(msg, args, index_sequence_for<MArgTs...>());
   }

private:
   template <typename Protocol, typename Msg, typename... MArgTs, size_t... Indexes>
   static Error deserialize_args_impl(Msg& msg, std::tuple<MArgTs...>& args, std::index_sequence<Indexes...> _) 
   {
      return ArgsSerializer<Protocol>::deserialize(msg, std::get<Indexes>(args)...);
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
