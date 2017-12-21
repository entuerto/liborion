//
// RequestMux.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <net/http/RequestMux.h>

#include <orion/Log.h>

#include <algorithm>

namespace orion
{
namespace net
{
namespace http
{

RequestMux::RequestMux()
{

}

RequestMux::~RequestMux()
{

}

void RequestMux::handle(const std::string& p, HandlerFunc h)
{
   
}

std::error_code RequestMux::operator()(const Request& req, Response& res)
{

}

} // http
} // net
} // orion
