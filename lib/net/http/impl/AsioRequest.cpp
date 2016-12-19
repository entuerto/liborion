//
// AsioRequest.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/impl/AsioRequest.h>

namespace orion
{
namespace net
{
namespace http
{
AsioRequest::AsioRequest() :
   Request()
{
   _header_streambuf = std::make_unique<asio::streambuf>();
   _body_streambuf   = std::make_unique<asio::streambuf>();
}

AsioRequest::AsioRequest(const std::string& method, 
                         const Url& url, 
                         const Version& version, 
                         const Header& header) :
   Request(method, url, version, header)
{
   _header_streambuf = std::make_unique<asio::streambuf>();
   _body_streambuf   = std::make_unique<asio::streambuf>();
}

AsioRequest::AsioRequest(AsioRequest&& rhs) :
   Request(std::move(rhs))
{
}

AsioRequest::~AsioRequest()
{
}

AsioRequest& AsioRequest::operator=(AsioRequest&& rhs)
{
   Request::operator=(std::move(rhs));

   return *this;   
}

void AsioRequest::build_header_buffer()
{
   auto u = url();

   header("Host", u.host());

   std::size_t body_size = static_cast<asio::streambuf*>(_body_streambuf.get())->size();

   if (body_size != 0)
      header("Content-Length", std::to_string(body_size));

   std::ostream o(_header_streambuf.get());

   auto v = http_version();

   // request-line = method SP request-target SP HTTP-version CRLF
   auto request_line = boost::format("%s %s HTTP/%d.%d") 
                          % method()
                          % u.path()
                          % v.major 
                          % v.minor;

   o << boost::str(request_line)
     << crlf;  

   for (const auto& item : _header)
   {
      o << item.first
        << ": "
        << item.second
        << crlf;
   } 

   o << crlf;
}

std::vector<asio::const_buffer> AsioRequest::buffers()
{
   build_header_buffer();

   std::vector<asio::const_buffer> b;
   b.push_back(static_cast<asio::streambuf*>(_header_streambuf.get())->data());
   b.push_back(static_cast<asio::streambuf*>(_body_streambuf.get())->data());

   return b;
}

std::size_t AsioRequest::size() const
{
   return static_cast<asio::streambuf*>(_header_streambuf.get())->size() + 
          static_cast<asio::streambuf*>(_body_streambuf.get())->size();
}

} // http
} // net
} // orion

