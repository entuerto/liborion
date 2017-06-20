//  Session.h
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_SESSION_H
#define ORION_NET_HTTP_SESSION_H

#include <orion/Orion-Stddefs.h>
#include <orion/net/Url.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/Utils.h>

#include <string>

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

   template<typename... Ts>
   Session(Ts&&... ts)
      : _url()
      , _params()
      , _header()
      , _timeout()
   {
      set_option(std::forward<Ts>(ts)...);
   }

   Session(Session&& rhs);
   virtual ~Session();

   Session& operator=(Session&& rhs);

   void set_option(const Url& url);

   void set_option(const Parameters& parameters);
   void set_option(Parameters&& parameters);

   void set_option(const Header& header);
   void set_option(const Timeout& timeout);
    
   template <typename T, typename... Ts>
   void set_option(Session& session, T&& t, Ts&&... ts)
   {
      set_option(std::forward<T>(t));
      set_option(std::forward<Ts>(ts)...);
   }

   virtual Response operator()(const Method& m);

protected:
   Url _url;
   Parameters _params;
   Header _header;
   Timeout _timeout;
};

    
template <typename T>
void set_option(Session& session, T&& t)
{
   session.set_option(std::forward<T>(t));
}
    
template <typename T, typename... Ts>
void set_option(Session& session, T&& t, Ts&&... ts)
{
   set_option(session, std::forward<T>(t));
   set_option(session, std::forward<Ts>(ts)...);
}
    
} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_SESSION_H
