//
// RequestHandler.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_REQUESTHANDLER_H
#define ORION_NET_HTTP_REQUESTHANDLER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Response.h>
#include <orion/net/http/Request.h>

namespace orion
{
namespace net
{
namespace http
{
class RequestHandler;

using Handlers = std::map<std::string, std::shared_ptr<RequestHandler>>;

//! Provides a simple, programmatically controlled HTTP protocol listener.
/*!
   Using the RequestHandler class, you can create a simple HTTP protocol listener that 
   responds to HTTP requests. The listener is active for the lifetime of the RequestHandler
   object and runs within your application.
 */
class API_EXPORT RequestHandler
{
public:
   virtual ~RequestHandler();

   std::string uri() const;

   virtual std::error_code on_request(const Request& request, Response& response);

protected:
   RequestHandler(const std::string& uri);

   virtual std::error_code on_get(const Request& request, Response& response);

   virtual std::error_code on_post(const Request& request, Response& response);

   virtual std::error_code on_put(const Request& request, Response& response);

   virtual std::error_code on_delete(const Request& request, Response& response);

private:
   std::string _uri;
};

} // http
} // net
} // orion

#endif // ORION_NET_HTTP_REQUESTHANDLER_H
