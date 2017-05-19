//
// AsioResponse.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/impl/AsioResponse.h>

#include <orion/Log.h>
#include <orion/net/http/Utils.h>

#include <boost/format.hpp>

#include <sstream>

using namespace orion::log;

namespace orion
{
namespace net
{
namespace http
{

AsioResponse::AsioResponse(StatusCode code)
   : Response(code)
{
   _header_streambuf = std::make_unique<asio::streambuf>();
   _body_streambuf   = std::make_unique<asio::streambuf>();
}

AsioResponse::AsioResponse(StatusCode code, const Version& version, const Header& header)
   : Response(code, version, header)
{
   _header_streambuf = std::make_unique<asio::streambuf>();
   _body_streambuf   = std::make_unique<asio::streambuf>();
}

AsioResponse::AsioResponse(AsioResponse&& rhs)
   : Response(std::move(rhs))
{
}

AsioResponse::~AsioResponse()
{
}

AsioResponse& AsioResponse::operator=(AsioResponse&& Rhs)
{
   Response::operator=(std::move(Rhs));

   return *this;
}

void AsioResponse::build_header_buffer()
{
   std::size_t body_size = static_cast<asio::streambuf*>(_body_streambuf.get())->size();

   if (_status_code == StatusCode::OK and body_size == 0)
      _status_code = StatusCode::NoContent;

   if (body_size != 0)
      header("Content-Length", std::to_string(body_size));

   std::ostream o(_header_streambuf.get());

   auto v = http_version();

   auto status_msg = boost::format("HTTP/%d.%d %s") % v.major % v.minor % status();

   o << boost::str(status_msg) << crlf;

   for (const auto& item : _header)
   {
      o << item.first << ": " << item.second << crlf;
   }

   o << crlf;
}

std::vector<asio::const_buffer> AsioResponse::buffers()
{
   build_header_buffer();

   std::vector<asio::const_buffer> b;
   b.push_back(static_cast<asio::streambuf*>(_header_streambuf.get())->data());
   b.push_back(static_cast<asio::streambuf*>(_body_streambuf.get())->data());

   return b;
}

std::size_t AsioResponse::size() const
{
   return static_cast<asio::streambuf*>(_header_streambuf.get())->size() +
          static_cast<asio::streambuf*>(_body_streambuf.get())->size();
}

} // http
} // net
} // orion
