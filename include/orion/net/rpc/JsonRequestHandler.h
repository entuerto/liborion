/*
 * JsonRequestHandler.h
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

#ifndef ORION_NET_RPC_JSONREQUESTHANDLER_H
#define ORION_NET_RPC_JSONREQUESTHANDLER_H

#include <memory>

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Response.h>
#include <orion/net/http/Request.h>
#include <orion/net/rpc/RequestHandler.h>
#include <orion/net/rpc/JsonError.h>
#include <orion/net/rpc/JsonMethod.h>

// Forward declares
namespace Json
{
class Value;
}

namespace orion
{
namespace net
{
namespace rpc
{
/// Provides a simple, programmatically controlled HTTP protocol listener.
/// 
/// Using the RequestListener class, you can create a simple HTTP protocol listener that 
/// responds to HTTP requests. The listener is active for the lifetime of the RequestListener
/// object and runs within your application.
/// 
class API_EXPORT JsonRequestHandler : public RequestHandler
{
public:
   JsonRequestHandler(const std::string& uri);
   virtual ~JsonRequestHandler();

protected:
   ///
   virtual std::error_code on_post(const http::Request& request, http::Response& response) override;

   //!
   Error process_method(Json::Value& json_request, Json::Value& json_result);

   //!
   Error validate(Json::Value& json_request);
};

//!
Json::Value make_response(const Json::Value& id, Json::Value& json_result);

//!
Json::Value make_response(const Json::Value& id, const Error& error);

} // rpc
} // net
} // orion

#endif // ORION_NET_RPC_JSONREQUESTHANDLER_H
