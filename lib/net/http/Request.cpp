//
// Request.cpp
//
//  Created by Tomas Palazuelos on 2016-11-07.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/net/http/Request.h>

#include <orion/StringUtils.h>

#include <asio.hpp>
#include <fmt/format.h>

#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace orion
{
namespace net
{
namespace http
{

Request::Request()
   : _method(Method::GET)
   , _url()
   , _version()
   , _header()
   , _should_keep_alive(false)
   , _upgrade(false)
   , _header_streambuf(std::make_unique<asio::streambuf>())
   , _body_streambuf(std::make_unique<asio::streambuf>())
{
}

Request::Request(const Method& method, const Url& url, const Version& version, const Header& header)
   : _method(method)
   , _url(url)
   , _version(version)
   , _header(header)
   , _should_keep_alive(false)
   , _upgrade(false)
   , _header_streambuf(std::make_unique<asio::streambuf>())
   , _body_streambuf(std::make_unique<asio::streambuf>())
{
}

Request::Request(Request&& rhs)
   : _method(std::move(rhs._method))
   , _url(std::move(rhs._url))
   , _version(std::move(rhs._version))
   , _header(std::move(rhs._header))
   , _should_keep_alive(std::move(rhs._should_keep_alive))
   , _upgrade(std::move(rhs._upgrade))
   , _header_streambuf(std::move(rhs._header_streambuf))
   , _body_streambuf(std::move(rhs._body_streambuf))
{
}

Request::~Request()
{
}

//! "GET", "POST", etc
Method Request::method() const
{
   return _method;
}

void Request::method(const Method& value)
{
   _method = value;
}

//! URL-decoded URI
Url Request::url() const
{
   return _url;
}

void Request::url(const Url& u)
{
   _url = u;
}

//! E.g. "1.0", "1.1"
Version Request::version() const
{
   return _version;
}

void Request::version(const Version& v)
{
   _version = v;
}

std::string Request::header(const std::string& name) const
{
   auto it = _header.find(name);

   if (it != _header.end())
      return it->second;

   return "";
}

void Request::header(const std::string& name, const std::string& value)
{
   _header[name] = value;
}

void Request::header(const Header& header)
{
   _header = header;
}

bool Request::should_keep_alive() const
{
   return _should_keep_alive;
}

void Request::should_keep_alive(bool value)
{
   _should_keep_alive = value;
}

bool Request::upgrade() const
{
   return _upgrade;
}

void Request::upgrade(bool value)
{
   _upgrade = value;
}

std::streambuf* Request::header_rdbuf() const
{
   const_cast<Request*>(this)->build_header_buffer();

   return _header_streambuf.get();
}

std::streambuf* Request::body_rdbuf() const
{
   return _body_streambuf.get();
}

Request& Request::operator=(Request&& rhs)
{
   _method            = std::move(rhs._method);
   _url               = std::move(rhs._url);
   _version           = std::move(rhs._version);
   _header            = std::move(rhs._header);
   _should_keep_alive = std::move(rhs._should_keep_alive);
   _upgrade           = std::move(rhs._upgrade);

   _header_streambuf = std::move(rhs._header_streambuf);
   _body_streambuf   = std::move(rhs._body_streambuf);
   return *this;
}

void Request::build_header_buffer() 
{
   auto u = url();

   header("Host", u.host());

   std::size_t body_size = static_cast<asio::streambuf*>(_header_streambuf.get())->size();

   if (body_size != 0)
      header("Content-Length", std::to_string(body_size));

   std::ostream o(_header_streambuf.get());

   auto v = version();

   // request-line = method SP request-target SP HTTP-version CRLF
   auto request_line = fmt::format("{0} {1} HTTP/{2}.{3}", method(), u.path(), v.major, v.minor);

   o << request_line << crlf;

   for (const auto& item : _header)
   {
      o << item.first << ": " << item.second << crlf;
   }

   o << crlf;
}

std::ostream& operator<<(std::ostream& o, const Request& r)
{
   static const int w = 18;

   Version v = r._version;

   o << std::left << std::boolalpha << "Request Record\n";

   o << std::setw(w) << "Method"
     << " : " << r._method << "\n"
     << std::setw(w) << "URL"
     << " : " << r._url.href() << "\n"
     << std::setw(w) << "Version"
     << " : " << v.major << "." << v.minor << "\n";

   for (const auto& item : r._header)
   {
      o << std::setw(w) << item.first << " : " << item.second << "\n";
   }

   o << std::setw(w) << "Should keep alive"
     << " : " << r._should_keep_alive << "\n"
     << std::setw(w) << "Upgrade"
     << " : " << r._upgrade << "\n";
   return o;
}

const orion::log::Record& operator<<(const orion::log::Record& rec, const Request& r)
{
   std::ostringstream outs;

   outs << "\n" << r;

   const_cast<orion::log::Record&>(rec).message(outs.str());

   return rec;
}

} // http
} // net
} // orion
