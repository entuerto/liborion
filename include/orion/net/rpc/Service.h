//
// Service.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//

#ifndef ORION_NET_RPC_SERVICE_H
#define ORION_NET_RPC_SERVICE_H

#include <map>

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/RequestHandler.h>
#include <orion/net/rpc/Method.h>
#include <orion/net/rpc/Params.h>
#include <orion/net/rpc/Result.h>

namespace orion
{
namespace net
{
namespace rpc
{
using Methods = std::map<std::string, std::shared_ptr<Method>>;

/// Provides RPC service.
///
class API_EXPORT Service 
{
public:
   NO_COPY(Service)

   Service(const std::string& name, 
           const std::string& desc, 
                 float        version, 
           const std::string& url);
   virtual ~Service();

   virtual std::string name() const;

   virtual std::string description() const;

   virtual float version() const;

   virtual std::string url() const;

   template <typename MethodHandler>
   void register_method(MethodHandler&& method);

   std::weak_ptr<Method> get_method(const std::string& name) const;

   std::error_code invoke_method(const std::string& name, const Params& params, Result& result);

private:
   std::string _name;
   std::string _description;
   float _version;
   std::string _url;

   Methods _methods;
};

//-------------------------------------------------------------------------------------------------

template <typename MethodHandler>
void Service::register_method(MethodHandler&& method)
{
   auto m = std::make_shared<MethodHandler>(std::move(method));

   _methods.emplace(std::make_pair(m->name(), m));
}

//-------------------------------------------------------------------------------------------------

API_EXPORT std::unique_ptr<http::RequestHandler> make_http_handler(std::unique_ptr<Service>&& service);

} // rpc
} // net
} // orion

#endif // ORION_NET_RPC_SERVICE_H
