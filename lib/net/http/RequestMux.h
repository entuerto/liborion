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

#include <orion/net/http/Utils.h>

namespace orion
{
namespace net
{
namespace http
{

/// HTTP request multiplexer
class RequestMux
{
public:
   RequestMux();
   ~RequestMux();

   void handle(const std::string& p, HandlerFunc h);

   std::error_code operator()(const Request& req, Response& res);

private:
};

} // http
} // net
} // orion
#endif // ORION_NET_HTTP_REQUESTMUX_H
