//
// Request.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Request.h>

#include <orion/String.h>

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
   : Message()
   , _method("GET")
   , _url()
{
}

Request::Request(const Method& method, const Url& url)
   : Message({1, 1})
   , _method(method)
   , _url(url)
{
}

Request::Request(const Method& method, const Url& url, const Version& version, const Header& header)
   : Message(version, header)
   , _method(method)
   , _url(url)
{
}

Request::Request(Request&& other) noexcept
   : Message(std::move(other))
   , _method(std::move(other._method))
   , _url(std::move(other._url))
{
}

Request::~Request() = default;

//! "GET", "POST", etc
const Method& Request::method() const
{
   return _method;
}

void Request::method(const Method& value)
{
   _method = value;
}

Url& Request::url()
{
   return _url;
}

const Url& Request::url() const
{
   return _url;
}

void Request::url(const Url& u)
{
   _url = u;
}

Request& Request::operator=(Request&& other) noexcept
{
   _method = std::move(other._method);
   _url    = std::move(other._url);

   Message::operator=(std::move(other));

   return *this;
}

void Request::build_header_buffer()
{
   init_header_buffer();

   auto u = url();

   header("Host", u.host());

   std::ostream o(header_buffer());

   auto v = version();

   auto request_line = fmt::format("{0} {1} HTTP/{2}.{3}", method(), u.path(), v.major, v.minor);

   o << request_line << crlf;

   Message::build_header_buffer();
}

std::ostream& operator<<(std::ostream& o, const Request& r)
{
   static const std::string text = R"(
Request Record
Method             : {}
URL                : {}
Version            : {}.{}
)";

   Version v = r.version();

   std::string upgrade = r.header(Field::Upgrade);

   o << std::left;

   o << fmt::format(text, r.method(), r.url().href(), v.major, v.minor);

   for (const auto& item : r.header())
   {
      o << std::setw(18) << item.first << " : " << item.second << "\n";
   }

   return o;
}

orion::log::Record& operator<<(orion::log::Record& rec, const Request& r)
{
   std::ostringstream outs;

   outs << "\n" << r;

   rec.message(outs.str());

   return rec;
}

} // namespace http
} // namespace net
} // namespace orion
