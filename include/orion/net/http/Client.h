//
// Client.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_CLIENT_H
#define ORION_NET_HTTP_CLIENT_H

#include <orion/Common.h>

#include <orion/AsyncService.h>
#include <orion/Log.h>

#include <orion/net/http/Session.h>
#include <orion/net/http/Utils.h>
#include <orion/net/Url.h>

#include <asio.hpp>

#include <future>
#include <string>

namespace orion
{
namespace net
{
namespace http
{

struct Call
{
   Method method;

   template<typename... Ts>
   std::future<Response> operator()(const Url& url, Ts&&... ts)
   {
      asio::io_context io_context;

      SyncSession session(io_context, std::forward<Ts>(ts)...);

      std::packaged_task<Response(SyncSession&)> task([&](SyncSession& s) { 
         return s.submit(method, url);
      });

      io_context.run();

      std::future<Response> result = task.get_future();

      task(session);

      return result;
   }
};

Call Get{Method{"GET"}};
Call Post{Method{"POST"}};
Call Put{Method{"PUT"}};
Call Patch{Method{"PATCH"}};
Call Delete{Method{"DELETE"}};


struct AsyncCall
{
   Method method;

   template<typename... Ts>
   void operator()(const Url& url, ResponseHandler rh, Ts&&... ts)
   {
      AsyncService as(1);

      auto session = std::make_shared<AsyncSession>(as.io_context(), std::forward<Ts>(ts)...);

      session->on_error([](const std::error_code& ec) {
         log::error(ec);
      });

      session->on_response(rh);

      //session->submit(Request{method, url});
      session->submit(method, url);

      as.run();
   }
};

AsyncCall AsyncGet{Method{"GET"}};
AsyncCall AsyncPost{Method{"POST"}};
AsyncCall AsyncPut{Method{"PUT"}};
AsyncCall AsyncPatch{Method{"PATCH"}};
AsyncCall AsyncDelete{Method{"DELETE"}};

} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_CLIENT_H
