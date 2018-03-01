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

#include <orion/Log.h>
#include <orion/Utils.h>
#include <orion/net/rpc/Error.h>
#include <orion/net/rpc/Method.h>
#include <orion/net/rpc/Result.h>
#include <orion/net/rpc/Traits.h>
#include <orion/net/tcp/Listener.h>
#include <orion/net/tcp/Session.h>

#include <fmt/format.h>

#include <functional>
#include <future>
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
template <typename Protocol>
class Service
{
public:
   using RequestType = typename Protocol::Request;
   using ResponseType = typename Protocol::Response;

   using HandlerFunc = std::function<Error(RequestType& req, ResponseType& res)>;
   using Methods     = std::map<std::string, HandlerFunc>;

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
   template<typename MethodT, typename FuncT>
   void register_method(FuncT func)
   {
      register_method_impl<MethodT>(std::move(func));
   }

   /// Add a class-method as a handler.
   template<typename MethodT, typename ClassT, typename RetT, typename... Args>
   void register_method(ClassT& object, RetT (ClassT::*method)(Args...))
   {
      register_method<MethodT>(MemberFnWrapper<ClassT, RetT, Args...>(object, method));
   }

   /// Remove the handler for the given method.
   /// A handler must currently be registered for this function.
   template<typename MethodT>
   void remove_method();

   /// Clear all methods.
   void clear_methods();

   void dump() const;

   std::error_code listen_and_serve(EndPoint endpoint)
   {
      auto handler = [&](asio::streambuf& in, asio::streambuf& out) {
         log::debug("handler", _src_loc);

         typename Protocol::Requests requests;

         Serializer<Protocol>::deserialize(in, requests);

         for (auto& request : requests)
         {
            auto search = _methods.find(request.method);
            if (search == _methods.end())
            {
               log::error("Method not found: ", request.method);
               continue;
            }

            ResponseType r = Protocol::make_response(request);

            auto error = search->second(request, r);
            if (error)
            {
               log::error("Error invoking method");
               continue;
            }

            Serializer<Protocol>::serialize(out, r);
         }

         return std::error_code();
      };

      std::make_shared<tcp::Listener>(_io_context, std::move(endpoint), handler)->start();

      std::error_code ec;

      _io_context.run(ec);

      return ec;
   }

protected:
   template<typename MethodT, typename FuncT>
   void register_method_impl(FuncT func)
   {

      //static_assert(ArgTypeCheck<TypeDeserializerCheck,
      //                           typename MethodT::Type,
      //                           typename HandlerTraits<FuncT>::Type>::value,
      //              "");

      _methods[MethodT::name()] = wrap_method<MethodT>(std::move(func));

      _method_info.emplace_back(MethodInfo{
         MethodT::name(), MethodT::description(), MethodT::version(), MethodT::prototype()});
   }

   // Wrap the given user handler in the necessary argument-deserialization code,
   // result-serialization code, and call to the launch policy (if present).
   template<typename MethodT, typename FuncT>
   HandlerFunc wrap_method(FuncT func)
   {
      return [this, func](RequestType& req, ResponseType& res) mutable -> Error {
         // Start by deserializing the arguments.
         using ArgsTuple = typename MethodArgsTuple<typename HandlerTraits<FuncT>::Type>::Type;

         auto args = std::make_shared<ArgsTuple>();

         auto error = HandlerTraits<typename MethodT::Type>::template deserialize_args<Protocol>(req, *args);
         if (error)
            return error;

         auto ret = HandlerTraits<typename MethodT::Type>::unpack_and_invoke(func, *args);

         error = Serializer<Protocol>::serialize(res.result, ret);
         if (error)
            return error;

         log::info("Value: ", ret);
         return Error();
      };
   }

private:
   std::string _name;
   std::string _description;
   Version _version;

   Methods _methods;

   std::vector<MethodInfo> _method_info;

   asio::io_context _io_context;
};

//-------------------------------------------------------------------------------------------------

template<typename Protocol>
class Dispatch
{
public:
   using Requests = typename Protocol::Requests;
   using Response = typename Protocol::Response;

   Dispatch(asio::io_context& io_context, EndPoint endpoint)
      : _io_context(io_context)
      , _endpoint(std::move(endpoint))
      , _session(io_context)
      , _requests() 
   {
      using namespace std::placeholders;

      _session.on_connect(std::bind(&Dispatch::on_connect, this, _1));
      _session.on_read(std::bind(&Dispatch::on_read, this, _1, _2));
      _session.on_write(std::bind(&Dispatch::on_write, this, _1, _2));
   }

   template<typename MethodT, typename... ArgTs>
   Error call(const ArgTs&... Args)
   {
      //static_assert(
      //   ArgTypeCheck<TypeSerializerCheck, typename MethodT::Type, void(ArgTs...)>::value, "");

      auto request = Protocol::make_request();

      auto error = MethodSerializer<Protocol>::template serialize<MethodT>(request, Args...);
      if (error)
      {
         return error;
      }

      _requests.emplace_back(std::move(request));

      _session.connect(EndPoint{"127.0.0.1"_ipv4, 9000});

      //std::future<Response> value = _read_promise.get_future();

      //value.wait();  // wait for result

      //Response res = value.get();

      log::debug("Result: ", &_response.result);

      return Error{};
   }

private:
   void on_connect(const std::error_code& ec)
   {
      if (ec)
      {
         log::error("Dispatch connection error: ", ec);
         return;
      }

      asio::streambuf buf;

      Serializer<Protocol>::serialize(buf, _requests);

      _session.write(&buf);
   }

   void on_read(const std::error_code& ec, asio::streambuf& buf)
   {
      if (ec)
      {
         log::error("Dispatch read error: ", ec);
         _session.close();
         return;
      }

      //log::debug(buf);
      
      //Response res;

      Serializer<Protocol>::deserialize(buf, _response);

      //_read_promise.set_value(res);

      _session.close();
   }

   void on_write(const std::error_code& ec, std::size_t bytes_sent)
   {
      if (ec)
      {
         log::error("Dispatch write error: ", ec);
         _session.close();
         return;
      }

      log::debug("Dispatch write bytes sent: ", bytes_sent);
   }

private:
   asio::io_context& _io_context;

   EndPoint _endpoint;

   tcp::Session _session;

   Requests _requests;

   std::promise<Response> _read_promise;

   Response _response;
};

} // rpc
} // net
} // orion

#include <orion/net/rpc/impl/Service.ipp>

#endif // ORION_NET_RPC_SERVICE_H
