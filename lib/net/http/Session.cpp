//
// Session.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/http/Session.h>

#include <utility>

namespace orion
{
namespace net
{
namespace http
{

Session::Session() :
   _url(),
   _params(),
   _header(),
   _timeout()
{
}

Session::Session(Session&& rhs) :
   _url(std::move(rhs._url)),
   _params(std::move(rhs._params)),
   _header(std::move(rhs._header)),
   _timeout(std::move(rhs._timeout)) 
{
}

Session::~Session()
{
}

Session& Session::operator=(Session&& rhs)
{
   _url     = std::move(rhs._url);
   _params  = std::move(rhs._params);
   _header  = std::move(rhs._header);
   _timeout = std::move(rhs._timeout); 
   return *this;   
}

void Session::set_option(const Url& url)
{
   _url = url;
}

void Session::set_option(const Parameters& params)
{
   _params = params;
}

void Session::set_option(Parameters&& params)
{
   _params = std::move(params);
}

void Session::set_option(const Header& header)
{
   _header = header;
}

void Session::set_option(const Timeout& timeout)
{
   _timeout = timeout;
}

} // http
} // net
} // orion
