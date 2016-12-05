//
// AsioRequest.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/AsioRequest.h>

namespace orion
{
namespace net
{
namespace http
{
AsioRequest::AsioRequest() :
   Request()
{
}

AsioRequest::AsioRequest(const std::string& method, 
                         const Url& url, 
                         const Version& version, 
                         const Header& header,
                               bool should_keep_alive,
                               bool upgrade) :
   Request(method, url, version, header, should_keep_alive, upgrade)
{
}

AsioRequest::AsioRequest(AsioRequest&& Other) :
   Request(std::move(Other))
{
}

AsioRequest::~AsioRequest()
{
}

AsioRequest& AsioRequest::operator=(AsioRequest&& Rhs)
{
   Request::operator=(std::move(Rhs));
   return *this;   
}

std::string AsioRequest::content() const
{
   return "";
}

} // http
} // net
} // orion

