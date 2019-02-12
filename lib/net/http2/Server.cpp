//
// Server.cpp
//
// Copyright (c) 2013-2019 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http2/Server.h>

#include "BasicServerImpl.h"

namespace orion
{
namespace net
{
namespace http2
{

Server make_server()
{
   return Server{std::make_shared<BasicServerImpl>()};
}

Server make_server(int backlog)
{
   return Server{std::make_shared<BasicServerImpl>(backlog)};
}

Server make_server(http::RequestMux&& mux)
{
   return Server{std::make_shared<BasicServerImpl>(std::move(mux))};
}

Server make_server(http::RequestMux&& mux, int backlog)
{
   return Server{std::make_shared<BasicServerImpl>(std::move(mux), backlog)};
}

} // namespace http2
} // namespace net
} // namespace orion
