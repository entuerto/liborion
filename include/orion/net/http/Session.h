//  Session.h
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_SESSION_H
#define ORION_NET_HTTP_SESSION_H

#include <string>

#include <orion/Orion-Stddefs.h>

#include <orion/net/Url.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>

namespace orion
{
namespace net
{
namespace http
{
///
///
///
class API_EXPORT Session
{
public:
   NO_COPY(Session);

   Session();
   Session(Session&& rhs);
   virtual ~Session();

   Session& operator=(Session&& rhs);

   void set_option(const Url& url);

   void set_option(const Parameters& parameters);
   void set_option(Parameters&& parameters);

   void set_option(const Header& header);
   void set_option(const Timeout& timeout);

   virtual Response operator()(const std::string& m);

protected:
   Url _url;
   Parameters _params;
   Header _header;
   Timeout _timeout;

};
} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_SESSION_H
