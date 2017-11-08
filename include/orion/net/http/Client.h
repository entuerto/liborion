//
// Client.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_CLIENT_H
#define ORION_NET_HTTP_CLIENT_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Session.h>
#include <orion/net/http/Utils.h>

#include <future>
#include <string>

namespace orion
{
namespace net
{
namespace http
{

struct call
{
   Method method;

   template<typename... Ts>
   std::future<Response> operator()(Ts&&... ts)
   {
      Session session(std::forward<Ts>(ts)...);

      std::packaged_task<Response(Session&)> task([&](Session& s) { return s(method); });

      std::future<Response> result = task.get_future();

      task(session);

      return result;
   }
};

call Get{Method::GET};
call Post{Method::POST};
call Put{Method::PUT};
call Patch{Method::PATCH};
call Delete{Method::DELETE};

} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_CLIENT_H
