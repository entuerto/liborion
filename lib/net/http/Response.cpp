//
// Response.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//

#include <orion/net/http/Response.h>

#include <asio.hpp>
#include <fmt/format.h>

#include <iomanip>
#include <sstream>

namespace orion
{
namespace net
{
namespace http
{

Response::Response()
   : _status_code(StatusCode::OK)
   , _version({1, 1})
   , _header()
   , _header_streambuf(std::make_unique<asio::streambuf>())
   , _body_streambuf(std::make_unique<asio::streambuf>())
{
}

Response::Response(StatusCode code)
   : _status_code(code)
   , _version({1, 1})
   , _header()
   , _header_streambuf(std::make_unique<asio::streambuf>())
   , _body_streambuf(std::make_unique<asio::streambuf>())
{
   /*
      if (_status_code >= StatusCode::BadRequest)
      {
         header("Connection", "close");
         header("X-Content-Type-Options", "nosniff");
      }
      else
         header("Content-Type", "text/plain; charset=utf-8");
   */
}

Response::Response(StatusCode code, const Version& version, const Header& header)
   : _status_code(code)
   , _version(version)
   , _header(header)
   , _header_streambuf(std::make_unique<asio::streambuf>())
   , _body_streambuf(std::make_unique<asio::streambuf>())
{
}

Response::Response(Response&& rhs)
   : _status_code(std::move(rhs._status_code))
   , _version(std::move(rhs._version))
   , _header(std::move(rhs._header))
   , _header_streambuf(std::move(rhs._header_streambuf))
   , _body_streambuf(std::move(rhs._body_streambuf))
{
}

Response::~Response()
{
}

Response& Response::operator=(Response&& rhs)
{
   _status_code = std::move(rhs._status_code);
   _version     = std::move(rhs._version);
   _header      = std::move(rhs._header);

   _header_streambuf = std::move(rhs._header_streambuf);
   _body_streambuf   = std::move(rhs._body_streambuf);

   return *this;
}

StatusCode Response::status_code() const
{
   return _status_code;
}

void Response::status_code(StatusCode sc)
{
   _status_code = sc;
}

std::string Response::status() const
{
   return boost::str(boost::format("%d %s") % _status_code % StatusText.at(_status_code));
}

Version Response::version() const
{
   return _version;
}

void Response::version(const Version& v)
{
   _version = v;
}

std::string Response::header(const std::string& name) const
{
   auto it = _header.find(name);

   if (it != _header.end())
      return it->second;

   return "";
}

void Response::header(const std::string& name, const std::string& value)
{
   _header[name] = value;
}

void Response::header(const Header& header)
{
   _header = header;
}

std::streambuf* Response::header_rdbuf() const
{
   const_cast<Response*>(this)->build_header_buffer();

   return _header_streambuf.get();
}

std::streambuf* Response::body_rdbuf() const
{
   return _body_streambuf.get();
}

void Response::build_header_buffer() 
{
   std::size_t body_size = static_cast<asio::streambuf*>(_body_streambuf.get())->size();

   if (_status_code == StatusCode::OK and body_size == 0)
      _status_code = StatusCode::NoContent;

   if (body_size != 0)
      header("Content-Length", std::to_string(body_size));

   std::ostream o(_header_streambuf.get());

   auto v = version();

   auto status_msg = boost::format("HTTP/%d.%d %s") % v.major % v.minor % status();

   o << boost::str(status_msg) << crlf;

   for (const auto& item : _header)
   {
      o << item.first << ": " << item.second << crlf;
   }

   o << crlf;
}

std::ostream& operator<<(std::ostream& o, const Response& r)
{
   auto v = r._version;

   auto status_msg = boost::format("HTTP/%d.%d %s") % v.major % v.minor % r.status();

   o << std::left << std::boolalpha << "\nResponse Header\n";

   o << boost::str(status_msg) << "\n";

   for (const auto& item : r._header)
   {
      o << item.first << ": " << item.second << "\n";
   }

   o << "\n";

   return o;
}

const log::Record& operator<<(const log::Record& rec, const Response& r)
{
   std::ostringstream outs;

   outs << r;

   const_cast<log::Record&>(rec).message(outs.str());

   return rec;
}

} // http
} // net
} // orion
