//
// RequestListener.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_REQUESTLISTENER_H
#define ORION_NET_HTTP_REQUESTLISTENER_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Response.h>
#include <orion/net/http/Request.h>

namespace orion
{
namespace net
{
namespace http
{
class RequestListener;

using Listeners = std::map<std::string, std::shared_ptr<RequestListener>>;

//! Provides a simple, programmatically controlled HTTP protocol listener.
/*!
   Using the RequestListener class, you can create a simple HTTP protocol listener that 
   responds to HTTP requests. The listener is active for the lifetime of the RequestListener
   object and runs within your application.
 */
class API_EXPORT RequestListener
{
public:
   virtual ~RequestListener();

   std::string uri() const;

   virtual std::error_code on_process_request(const Request& request, Response& response);

protected:
   RequestListener(const std::string& uri);

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

#endif
