//
// Response.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/net/http/Response.h>

#include <fmt/format.h>

#include <iomanip>
#include <ostream>
#include <sstream>

namespace orion
{
namespace net
{
namespace http
{

Response::Response()
   : Message()
   , _status_code(StatusCode::OK)
{
}

Response::Response(StatusCode code)
   : Message({1, 1})
   , _status_code(code)
{
}

Response::Response(StatusCode code, const Version& version, const Header& header)
   : Message(version, header)
   , _status_code(code)
{
}

Response::Response(Response&& other) noexcept
   : Message(std::move(other))
   , _status_code(std::move(other._status_code))
{
}

Response::~Response() = default;

Response& Response::operator=(Response&& other) noexcept
{
   _status_code = std::move(other._status_code);

   Message::operator=(std::move(other));

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
   return fmt::format("{0} {1}", _status_code, status_text().at(_status_code));
}

void Response::build_header_buffer()
{
   init_header_buffer();

   if (_status_code == StatusCode::OK and body_size() == 0)
      _status_code = StatusCode::NoContent;

   std::ostream o(header_buffer());

   auto v = version();

   auto status_msg = fmt::format("HTTP/{0}.{1} {2}", v.major, v.minor, status());

   o << status_msg << crlf;

   Message::build_header_buffer();
}

std::ostream& operator<<(std::ostream& o, const Response& r)
{
   auto v = r.version();

   auto status_msg = fmt::format("HTTP/{0}.{1} {2}", v.major, v.minor, r.status());

   o << std::left << std::boolalpha << "\nResponse Header\n";

   o << status_msg << "\n";

   for (const auto& item : r.header())
   {
      o << item.first << ": " << item.second << "\n";
   }

   o << "\n";

   return o;
}

log::Record& operator<<(log::Record& rec, const Response& r)
{
   std::ostringstream outs;

   outs << r;

   rec.message(outs.str());

   return rec;
}

} // http
} // net
} // orion
