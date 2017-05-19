//  Cookies.h
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_COOKIES_H
#define ORION_NET_HTTP_COOKIES_H

#include <orion/Orion-Stddefs.h>

#include <chrono>
#include <string>

namespace orion
{
namespace net
{
namespace http
{

struct Cookie
{
   std::string name;
   std::string value;

   /// The scope of each cookie is limited to a set of paths, controlled by
   /// the Path attribute.
   std::string path;
   /// The Domain attribute specifies those hosts to which the cookie will
   /// be sent.
   std::string domain;
   /// The Expires attribute indicates the maximum lifetime of the cookie,
   /// represented as the date and time at which the cookie expires.
   std::chrono::steady_clock::time_point expires;
   /// The Max-Age attribute indicates the maximum lifetime of the cookie,
   /// represented as the number of seconds until the cookie expires.
   std::chrono::seconds max_age;
   /// The Secure attribute limits the scope of the cookie to "secure"
   /// channels (where "secure" is defined by the user agent).
   bool secure;
   /// The HttpOnly attribute limits the scope of the cookie to HTTP
   /// requests.
   bool http_only;
};

/*
   Notes

   The Max-Age Attribute:
   If a cookie has both the Max-Age and the Expires attribute, the Max-
   Age attribute has precedence and controls the expiration date of the
   cookie.  If a cookie has neither the Max-Age nor the Expires
   attribute, the user agent will retain the cookie until "the current
   session is over" (as defined by the user agent).

   The Path Attribute:
   If the server omits the Path attribute, the user agent will use the
   "directory" of the request-uri's path component as the default value.
   (See Section 5.1.4 for more details.)
*/

} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_CLIENT_H
