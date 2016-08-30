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

#include <orion/Orion-Stddefs.h>
#include <orion/MemoryUtils.h>
#include <orion/ws/Response.h>
#include <orion/ws/Request.h>
#include <orion/ws/RequestListener.h>
#include <orion/ws/RpcMethod.h>

namespace orion
{
namespace ws
{
typedef std::map<std::string, std::unique_ptr<RpcMethod>> RpcMethodMap;

//! Provides RPC protocol listener.
/*!
   Using the RpcRequestListener class, you can create a simple RPC protocol listener that 
   responds to RPC requests. 
 */
class API_EXPORT RpcRequestListener : public RequestListener
{
public:
   virtual ~RpcRequestListener();

   void register_method(const std::string& name, std::unique_ptr<RpcMethod>&& method);

   RpcMethod* get_method(const std::string& name) const;

protected:
   RpcRequestListener(const std::string& uri);

   virtual std::unique_ptr<Response> on_get(const Request* request) override;

   virtual std::unique_ptr<Response> on_post(const Request* request) override;

   virtual std::unique_ptr<Response> on_put(const Request* request) override;

   virtual std::unique_ptr<Response> on_delete(const Request* request) override;

protected:
   RpcMethodMap _RpcMethods;

};

} // ws
} // orion

#endif

