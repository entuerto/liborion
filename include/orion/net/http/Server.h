//
// Server.h
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#ifndef ORION_NET_HTTP_SERVER_H
#define ORION_NET_HTTP_SERVER_H

#include <memory>
#include <string>

#include <orion/Orion-Stddefs.h>
#include <orion/net/Server.h>
#include <orion/net/http/RequestListener.h>

namespace orion
{
namespace net
{
namespace http
{
//! This class provides an embedded HTTP Server
/*!
   This class provides an embedded HTTP Server to handle incoming Requests
   and send HTTP 1.1/2 valid responses.
 */
class API_EXPORT Server : public net::Server
{
public:
   NO_COPY(Server)

   virtual ~Server();

   virtual int port() const;

   virtual void add_request_listener(std::unique_ptr<RequestListener>&& listener);

   bool is_running() const override;
   
   /// Create a Http server object
   static std::unique_ptr<http::Server> create();

protected:
   Server();

   int _port;
   bool _is_running;
   Listeners _RequestListeners;
};

} // http
} // net
} // orion
#endif 
