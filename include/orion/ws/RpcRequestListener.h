/*
 * RpcRequestListener.h
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

#ifndef ORION_WS_RPCREQUESTLISTENER_H
#define ORION_WS_RPCREQUESTLISTENER_H

#include <map>
#include <memory>

#include <orion/Orion-Stddefs.h>
#include <orion/net/Response.h>
#include <orion/net/Request.h>
#include <orion/net/RequestListener.h>
#include <orion/ws/RpcMethod.h>

namespace orion
{
namespace ws
{
typedef std::map<std::string, std::unique_ptr<RpcMethod>> RpcMethods;

//! Provides RPC protocol listener.
/*!
   Using the RpcRequestListener class, you can create a simple RPC protocol listener that 
   responds to RPC requests. 
 */
class API_EXPORT RpcRequestListener : public net::RequestListener
{
public:
   virtual ~RpcRequestListener();

   void register_method(const std::string& name, std::unique_ptr<RpcMethod>&& method);

   RpcMethod* get_method(const std::string& name) const;

protected:
   RpcRequestListener(const std::string& uri);

   virtual std::unique_ptr<net::Response> on_get(const net::Request* request) override;

   virtual std::unique_ptr<net::Response> on_post(const net::Request* request) override;

   virtual std::unique_ptr<net::Response> on_put(const net::Request* request) override;

   virtual std::unique_ptr<net::Response> on_delete(const net::Request* request) override;

protected:
   RpcMethods _rpc_methods;

};

} // ws
} // orion

#endif

