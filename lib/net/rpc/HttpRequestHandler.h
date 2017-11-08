//
// HttpRequestHandler.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_RPC_HTTPREQUESTHANDLER_H
#define ORION_NET_RPC_HTTPREQUESTHANDLER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/rpc/Service.h>

#include <memory>

namespace orion
{
namespace net
{
namespace rpc
{
class Service;

/// Provides RPC protocol handler.
///
/// Using the RpcRequestListener class, you can create a simple RPC protocol handler that
/// responds to RPC requests.
///
class HttpRequestHandler
{
public:
   NO_COPY(HttpRequestHandler)

   HttpRequestHandler(std::unique_ptr<Service>&& service);
   virtual ~HttpRequestHandler() = default;


private:
   std::unique_ptr<Service> _service;
};

} // rpc
} // net
} // orion

#endif // ORION_NET_RPC_HTTPREQUESTHANDLER_H
