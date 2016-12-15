//
// MongooseResponse.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/MongooseResponse.h>

#include <sstream>

#include <orion/Logging.h>
#include <orion/net/http/Utils.h>

#include <boost/format.hpp>

using namespace orion::logging;

namespace orion
{
namespace net
{
namespace http
{

MongooseResponse::MongooseResponse(StatusCode code) :
   Response(code),
   _header_streambuf(std::make_unique<std::stringbuf>()),
   _body_streambuf(std::make_unique<std::stringbuf>())
{
}

MongooseResponse::MongooseResponse(StatusCode code, const Version& version, const Header& header) :
   Response(code, version, header),
   _header_streambuf(std::make_unique<std::stringbuf>()),
   _body_streambuf(std::make_unique<std::stringbuf>())
{
}

MongooseResponse::MongooseResponse(MongooseResponse&& Other) :
   Response(std::move(Other)),
   _header_streambuf(std::move(Other._header_streambuf)),
   _body_streambuf(std::move(Other._body_streambuf))
{
}

MongooseResponse::~MongooseResponse()
{
}

MongooseResponse& MongooseResponse::operator=(MongooseResponse&& Rhs)
{
   Response::operator=(std::move(Rhs));

   _header_streambuf = std::move(Rhs._header_streambuf);
   _body_streambuf   = std::move(Rhs._body_streambuf);

   return *this;   
}

std::streambuf* MongooseResponse::rdbuf() const
{
   return _body_streambuf.get();
}

void MongooseResponse::build_header_buffer()
{
   std::size_t body_size = _body_streambuf->str().size();

   LOG(Debug2) << "Response body size: " << int(body_size);

   if (_status_code == StatusCode::OK and body_size == 0)
      _status_code = StatusCode::NoContent;

   if (body_size != 0)
      header("Content-Length", std::to_string(body_size));

   std::ostream o(_header_streambuf.get());

   auto v = http_version();

   auto status_msg = boost::format("HTTP/%d.%d %s") % v.major % v.minor % status();

   o << boost::str(status_msg)
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

std::string MongooseResponse::buffer() 
{
   build_header_buffer();

   return _header_streambuf->str() + _body_streambuf->str();
}

} // http
} // net
} // orion

