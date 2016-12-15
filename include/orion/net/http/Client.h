//  Client.h
//
//  Created by Tomas Palazuelos on 2016-12-12.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#ifndef ORION_NET_HTTP_CLIENT_H
#define ORION_NET_HTTP_CLIENT_H

#include <future>
#include <string>

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Session.h>

namespace orion
{
namespace net
{
namespace http
{

namespace priv 
{

template <typename T>
void set_option(Session& session, T&& t) 
{
   session.set_option(std::forward<T>(t));
}

template <typename T, typename... Ts>
void set_option(Session& session, T&& t, Ts&&... ts) 
{
   set_option(session, std::forward<Ts>(t));
   set_option(session, std::forward<Ts>(ts)...);
}

} // namespace priv

// Get method
template <typename... Ts>
std::future<Response> get(Ts&&... ts) 
{
   Session session;
   priv::set_option(session, std::forward<Ts>(ts)...);

   std::packaged_task<Response(Session&)> task([](Session& s) {
      return s(Method::Get); 
   });

   std::future<Response> result = task.get_future();
 
   task(session);

   return result;
}

// Post methods
template <typename... Ts>
std::future<Response> post(Ts&&... ts) 
{
   Session session;
   priv::set_option(session, std::forward<Ts>(ts)...);

   std::packaged_task<Response(Session&)> task([](Session& s) {
      return s(Method::Post); 
   });

   std::future<Response> result = task.get_future();
 
   task(session);

   return result;
}

} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_CLIENT_H
