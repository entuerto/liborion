//
// RequestMux.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#ifndef ORION_NET_HTTP_REQUESTMUX_H
#define ORION_NET_HTTP_REQUESTMUX_H

#include <orion/Orion-Stddefs.h>

#include <orion/net/http/Request.h>
#include <orion/net/http/Response.h>
#include <orion/net/http/StatusCode.h>
#include <orion/net/http/Utils.h>

#include <map>
#include <unordered_map>

namespace orion
{
namespace net
{
namespace http
{

/// HTTP request multiplexer
class API_EXPORT RequestMux
{
public:
   DEFAULT_COPY(RequestMux)
   DEFAULT_MOVE(RequestMux)

   RequestMux();
   ~RequestMux();

   /// handle registers the handler for the given method and pattern.
   void handle(Method method, const std::string& pattern, HandlerFunc h);

   /// Register user defined status code handlers.
   void handle(StatusCode status_code, HandlerFunc h);

   HandlerFunc handler(const Request& req) const;

   HandlerFunc match(Method method, const std::string& path) const;

   std::error_code operator()(const Request& req, Response& res);

private:
   HandlerFunc status_handler(StatusCode status_code) const; 

   struct Entry 
   {
      bool user_defined;
      std::string pattern;
      std::map<Method, HandlerFunc> handlers;
   };

   std::unordered_map<std::string, Entry> _mux;
   std::map<StatusCode, HandlerFunc> _status_handlers;
};

} // namespace http
} // namespace net
} // namespace orion
#endif // ORION_NET_HTTP_REQUESTMUX_H
