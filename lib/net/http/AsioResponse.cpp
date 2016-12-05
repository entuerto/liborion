//
// AsioResponse.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <net/http/AsioResponse.h>

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

AsioResponse::AsioResponse(StatusCode code) :
   Response(code),
   _header_streambuf(std::make_unique<asio::streambuf>()),
   _body_streambuf(std::make_unique<asio::streambuf>())
{
}

AsioResponse::AsioResponse(StatusCode code, const Version& version) :
   Response(code, version),
   _header_streambuf(std::make_unique<asio::streambuf>()),
   _body_streambuf(std::make_unique<asio::streambuf>())
{
}

AsioResponse::AsioResponse(AsioResponse&& Other) :
   Response(std::move(Other)),
   _header_streambuf(std::move(Other._header_streambuf)),
   _body_streambuf(std::move(Other._body_streambuf))
{
}

AsioResponse::~AsioResponse()
{
}

AsioResponse& AsioResponse::operator=(AsioResponse&& Rhs)
{
   Response::operator=(std::move(Rhs));

   _header_streambuf = std::move(Rhs._header_streambuf);
   _body_streambuf   = std::move(Rhs._body_streambuf);

   return *this;   
}

std::string AsioResponse::content() const
{
   return asio::buffer_cast<const char*>(_body_streambuf->data());
}

std::streambuf* AsioResponse::rdbuf() const
{
   return _body_streambuf.get();
}

void AsioResponse::build_header_buffer()
{
   std::size_t body_size = _body_streambuf->size();

   LOG(Debug) << "Response body size: " << int(body_size);

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

std::vector<asio::const_buffer> AsioResponse::buffers()
{
   build_header_buffer();

   std::vector<asio::const_buffer> b;
   b.push_back(_header_streambuf->data());
   b.push_back(_body_streambuf->data());

   return b;
}

std::size_t AsioResponse::size() const
{
   return _header_streambuf->size() + _body_streambuf->size();
}
/*
std::ostream& operator<<(std::ostream& o, const AsioResponse& r)
{
   auto v = r.http_version();

   auto status_msg = boost::format("HTTP/%d.%d %s") % v.major % v.minor % r.status();

   o << boost::str(status_msg)
     << crlf;  

   for (const auto& item : r._header)
   {
      o << item.first
        << ": "
        << item.second
        << crlf;
   } 

   o << crlf;
   o << r.content();

   return o;
}
*/
/*
AsioResponseConstBuffer::AsioResponseConstBuffer(const AsioResponse& data) : 
   _data(),
   _buffer(),
   _size(0)
{
   std::ostringstream ostr;

   ostr << data;

   _data = std::make_shared<std::string>(ostr.str());
   _buffer = asio::buffer(*_data);
   _size = _data->size();
}
*/
} // http
} // net
} // orion

