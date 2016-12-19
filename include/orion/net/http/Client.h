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


struct call
{
   std::string method;

   template <typename... Ts>
   std::future<Response> operator()(Ts&&... ts) 
   {
      Session session(std::forward<Ts>(ts)...);

      std::packaged_task<Response(Session&)> task([&](Session& s) {
         return s(method); 
      });

      std::future<Response> result = task.get_future();
    
      task(session);

      return result;
   }
};

call Get{Method::Get};
call Post{Method::Post};
call Put{Method::Put};
call Patch{Method::Patch};
call Delete{Method::Delete};


} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_CLIENT_H
