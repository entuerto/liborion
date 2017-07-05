/*
 * HttpRequestHandler.h
 *
 * Copyright 2013 tomas <tomasp@videotron.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

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
