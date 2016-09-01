/*
 * JsonRpcRequestListener.h
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

#ifndef ORION_WS_JSONRPCREQUESTLISTENER_H
#define ORION_WS_JSONRPCREQUESTLISTENER_H

#include <memory>

#include <orion/Orion-Stddefs.h>
#include <orion/net/Response.h>
#include <orion/net/Request.h>
#include <orion/ws/RpcRequestListener.h>
#include <orion/ws/JsonRpcError.h>

// Forward declares
namespace Json
{
class Value;
}

namespace orion
{
namespace ws
{
//! Provides a simple, programmatically controlled HTTP protocol listener.
/*!
   Using the RequestListener class, you can create a simple HTTP protocol listener that 
   responds to HTTP requests. The listener is active for the lifetime of the RequestListener
   object and runs within your application.
 */
class API_EXPORT JsonRpcRequestListener : public RpcRequestListener
{
public:
   JsonRpcRequestListener(const std::string& uri);
   virtual ~JsonRpcRequestListener();

   static std::unique_ptr<JsonRpcRequestListener> create(const std::string& uri);

protected:
   ///
   virtual std::unique_ptr<net::Response> on_post(const net::Request* request) override;

   //!
   std::unique_ptr<JsonRpcError> process_method(Json::Value& json_request, Json::Value& json_result);

   //!
   std::unique_ptr<JsonRpcError> validate(Json::Value& json_request);

   //!
   Json::Value make_json_rpc_response(const Json::Value& id, Json::Value& json_result);

   //!
   Json::Value make_json_rpc_response(const Json::Value& id, const JsonRpcError* error);

};

} // ws
} // orion

#endif

